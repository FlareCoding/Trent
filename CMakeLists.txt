cmake_minimum_required(VERSION 3.0)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Production")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (APPLE)
    set(CMAKE_MACOSX_RPATH OFF)
endif()

add_definitions(-DUNICODE -D_UNICODE)

project(Trent)
add_subdirectory(runtime)
add_subdirectory(interpreter)

add_library(
    TrentRuntime SHARED
    
    ${RUNTIME_HEADERS}
    ${RUNTIME_SOURCES}
)
target_include_directories(TrentRuntime PUBLIC "runtime")
target_compile_definitions(TrentRuntime PRIVATE TR_BUILD)

add_executable(
    TrentInterpreter
    
    ${INTERPRETER_HEADERS}
    ${INTERPRETER_SOURCES}
)

target_include_directories(TrentInterpreter PUBLIC "interpreter")
target_include_directories(TrentInterpreter PUBLIC "runtime")
target_link_libraries(TrentInterpreter PUBLIC TrentRuntime)

add_custom_target(Tests
    SOURCES
        tests/main.trent
        tests/loops.trent
        tests/mem_test.trent
        tests/fibb.trent
)
