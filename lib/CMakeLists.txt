project(lib)

set(HEADER_FILES
        dfs-constant-memory.h
        dfs-linear-memory.h
        generator.h
        utilities.h
        )

set(SOURCE_FILES
        dfs-constant-memory.cpp
        dfs-linear-memory.cpp
        generator.cpp
        utilities.cpp
        )

add_library(lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
