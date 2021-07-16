cmake_minimum_required(VERSION 3.0)

set(CMAKE_CURRENT_SOURCE_DIR interpreter/TrentStd)

set(
    TRENTSTD_HEADERS

    interpreter/TrentStd/TrentStandardLibrary.h

    PARENT_SCOPE
)

set(
    TRENTSTD_SOURCES

    interpreter/TrentStd/TrentStandardLibrary.cpp

    PARENT_SCOPE
)
