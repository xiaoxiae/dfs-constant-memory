project(Array)

set(SOURCE_FILES
        dfs-constant-memory.cpp
        dfs-linear-memory.cpp
        generator.cpp
        utilities.cpp
        )

add_library(lib STATIC ${SOURCE_FILES})
