cmake_minimum_required(VERSION 3.0)

set(CMAKE_CURRENT_SOURCE_DIR interpreter)

add_subdirectory(TrentStd)
add_subdirectory(AST)
add_subdirectory(Parser)

set(
    INTERPRETER_HEADERS

    ${TRENTSTD_HEADERS}
    ${TRENTAST_HEADERS}
    ${TRENTPARSER_HEADERS}

    interpreter/TrentInterpreter.h

    PARENT_SCOPE
)

set(
    INTERPRETER_SOURCES

    ${TRENTSTD_SOURCES}
    ${TRENTAST_SOURCES}
    ${TRENTPARSER_SOURCES}

    interpreter/TrentInterpreter.cpp
    interpreter/main.cpp

    PARENT_SCOPE
)