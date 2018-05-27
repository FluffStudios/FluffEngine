project (fluff-core)

set (FC_SOURCE source)

file (GLOB_RECURSE sources "${FC_SOURCE}/*.cpp")

add_library (fluff-core
        "${sources}"
)

target_include_directories(fluff-core PUBLIC includes ../fluff-utilities/includes ../depend/include)