from enum import Enum

import requests
import os
import uuid
import sys
import shutil
import zipfile
import subprocess
import glob

import datetime
import time

DEPENDENCIES_PATH = 'Engine/Dependencies'
DEPENDENCY_BINARY_DIR = 'Engine/Dependencies/bin'

def display_progress_bar(count, total, status=''):
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))

    percents = round(100.0 * count / float(total), 1)
    bar = '=' * filled_len + '-' * (bar_len - filled_len)

    sys.stdout.write('[%s] %s%s ...%s\r' % (bar, percents, '%', status))
    sys.stdout.flush()

class SourceType(Enum):
    """
    A git repository containing the library source code.
    """
    Git = 1

    """
    An archived file containing the library source code.
    """
    Archive = 2

class UnsupportedSourceTypeError(Exception):
    def __init__(self, source_type):
        super(UnsupportedSourceTypeError, self).__init__(f'Unsupported source type: \'{source_type.name}\'')

class RepositorySource(object):
    def __init__(self, name, url, url_type=SourceType.Git):
        self.url = url
        self.name = name
        self.type = url_type

    def get_destination_directory(self, container_directory):
        return os.path.join(container_directory, self.name)

    """
    Downloads and extracts the repository source into
    a new directory inside the container_directory.
    """
    def get(self, container_directory):
        destination_path = self.get_destination_directory(container_directory)

        if os.path.isdir(destination_path):
            shutil.rmtree(destination_path)

        if self.type == SourceType.Git:
            raise UnsupportedSourceTypeError(self.type)
        elif self.type == SourceType.Archive:
            # Extract and build filesystem
            archive_name = str(uuid.uuid4())
            file_name = os.path.join(container_directory, archive_name)

            with open(file_name, 'wb') as file_handle:
                print(f'{self.name}: Downloading archive ({self.url})')

                start_time = time.time()
                response = requests.get(self.url, stream=True)
                total_length = response.headers.get('content-length')

                # no content length header
                if total_length is None: 
                    file_handle.write(response.content)
                else:
                    current_length = 0
                    total_length = int(total_length)
                    for data in response.iter_content(chunk_size=4096):
                        current_length += len(data)
                        file_handle.write(data)

                        elapsed = time.time() - start_time
                        speed = elapsed / current_length

                        time_left = datetime.timedelta(seconds=round((speed * total_length) - elapsed)) 
                        display_progress_bar(current_length, total_length, str(time_left))
                    
                    print()
                    
            if not zipfile.is_zipfile(file_name):
                raise zipfile.BadZipFile()

            print(f'{self.name}: Finished downloading archive ({self.url})')
            print(f'{self.name}: Extracting archive')
            with zipfile.ZipFile(file_name) as zip_file:
                total_files = len(zip_file.namelist())
                extracted_files = 0

                for name in zip_file.namelist():
                    zip_file.extract(name, destination_path)
                    extracted_files += 1

                    display_progress_bar(extracted_files, total_files, os.path.basename(name))
                
                print('\n')

            cmakefile_dir = str()
            for root, dirs, files in os.walk(destination_path, topdown=True):
                if 'CMakeLists.txt' in files:
                    cmakefile_dir = os.path.join(root)
                    break

            print(f'{self.name}: Located CMakeLists.txt')
            if cmakefile_dir != destination_path:
                files = os.listdir(cmakefile_dir)
                for file in files:
                    shutil.move(os.path.join(cmakefile_dir, file), destination_path)

                os.rmdir(cmakefile_dir)
                print(f'{self.name}: Moved directory containing CMakeLists.txt to top-level root directory of dependency')

            os.remove(file_name)        

            source_dependency_path = os.path.join(DEPENDENCIES_PATH, self.name)
            if os.path.isdir(source_dependency_path):
                shutil.rmtree(source_dependency_path)

            while not os.path.isdir(source_dependency_path):
                os.mkdir(source_dependency_path)

            # locate includes
            print(f'{self.name}: Locating include directory')

            include_dir = str()

            real_destination_path = os.path.realpath(destination_path)
            destination_dir_contents = os.listdir(real_destination_path)
            for item in destination_dir_contents:
                real_item_path = os.path.join(real_destination_path, item)
                if os.path.isdir(real_item_path):
                    if item == 'include':
                        include_dir = real_item_path
                        print(f'{self.name}: Located include directory ({include_dir})')
                        break

            shutil.copytree(include_dir, os.path.join(source_dependency_path, 'include'))
            print(f'{self.name}: Copied include directory')

            def cmake_build(config, arch, include_pdb=False): 
                # Generate project and build
                print(f'{self.name}: Generating CMake project')
                build_dir = os.path.join(destination_path, f'cmake-build-{arch}')
                if os.path.isdir(build_dir):
                    shutil.rmtree(build_dir)

                os.mkdir(build_dir)

                cmake_command = ['cmake', real_destination_path]
                if os.name == 'nt':
                    # cmake_command = ['cmake', '-G', f'\"Visual Studio 15 2017{ " " + arch if arch == "Win64" else ""}\"', f'\"{real_destination_path}\"']
                    os.system(f'cd {build_dir} && cmake -G \"Visual Studio 15 2017{ " " + arch if arch == "Win64" else ""}\" \"{real_destination_path}\"')
                else:
                    subprocess.run(cmake_command, shell=False, cwd=build_dir)
                
                print(f'{self.name}: Finished generating CMake project')

                print(f'{self.name}: Building ({config})')
                subprocess.run(['cmake', '--build', '.', '--config', config], shell=False, cwd=build_dir)
                print(f'{self.name}: Completed building ({config})')

                def harvest_files(extension):
                    files = list(glob.iglob(os.path.join(build_dir, f'**/{config}/*.{extension}'), recursive=True))
                    file_map = {}
                    for file in files:
                        basename = os.path.basename(file)
                        if basename not in file_map:
                            file_map[basename] = file

                    return file_map.values()
                    

                def find_target_file(files):
                    for file in files:
                        if self.name in file:
                            return file

                generic_arch = arch
                if arch == 'Win64': 
                    generic_arch = 'x64'
                 
                target_lib = find_target_file(harvest_files('lib'))                       
                if target_lib != None:
                    lib_dir = os.path.join(source_dependency_path, f'lib/{config}/{generic_arch}')
                    if os.path.isdir(lib_dir):
                        shutil.rmtree(lib_dir)

                    while not os.path.isdir(lib_dir):
                        os.makedirs(lib_dir)

                    shutil.copy(target_lib, lib_dir)

                target_dll = find_target_file(harvest_files('dll'))
                if target_dll != None:
                    bin_dir = os.path.join(DEPENDENCY_BINARY_DIR, f'{config}/{generic_arch}')
                    if not os.path.isdir(bin_dir):
                        os.makedirs(bin_dir)

                    shutil.copy(target_dll, bin_dir)

                    if include_pdb:
                        target_pdb = find_target_file(harvest_files('pdb'))
                        shutil.copy(target_pdb, bin_dir)

            # cmake_build('Debug', 'Win32', True)
            cmake_build('Release', 'Win32')
        
            # cmake_build('Debug', 'Win64', True)
            cmake_build('Release', 'Win64')
                        
# define our sources
repository_sources = [
    RepositorySource('assimp', 'https://github.com/assimp/assimp/archive/v4.1.0.zip', url_type=SourceType.Archive)
]

DEPENDENCY_TEMP_DIR = './dependency_temp'

def clean_temp_dir():
    # Clean our temp dependency directory
    # If it exists, remove it!
    if os.path.isdir(DEPENDENCY_TEMP_DIR):
        shutil.rmtree(DEPENDENCY_TEMP_DIR, ignore_errors=True)

while os.path.isdir(DEPENDENCY_TEMP_DIR):
    clean_temp_dir()

time.sleep(1)
os.mkdir(DEPENDENCY_TEMP_DIR)

while os.path.isdir(DEPENDENCY_BINARY_DIR):
    shutil.rmtree(DEPENDENCY_BINARY_DIR, ignore_errors=True)

os.mkdir(DEPENDENCY_BINARY_DIR)

for source in repository_sources:
    source.get(DEPENDENCY_TEMP_DIR)

clean_temp_dir()