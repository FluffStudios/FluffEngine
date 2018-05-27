project (fluff-utilties)

set (FU_SOURCE source)

file (GLOB_RECURSE sources "${FU_SOURCE}/*.cpp")

add_library (fluff-utilities
        "${sources}"
)

target_include_directories(fluff-utilities PUBLIC includes ../depend/include)