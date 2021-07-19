cmake_minimum_required(VERSION 3.0)

set(CMAKE_CURRENT_SOURCE_DIR runtime)

set(
    RUNTIME_HEADERS

    runtime/Trent.h
    runtime/TrentCore.h
    runtime/TrentRuntime.h
    runtime/TrentException.h
    runtime/TrentObject.h
    runtime/TrentInteger.h
    runtime/TrentString.h
    runtime/TrentTuple.h
    runtime/TrentBoolean.h
    runtime/TrentFloat.h

    PARENT_SCOPE
)

set(
    RUNTIME_SOURCES

    runtime/TrentRuntime.cpp
    runtime/TrentException.cpp
    runtime/TrentObject.cpp
    runtime/TrentInteger.cpp
    runtime/TrentString.cpp
    runtime/TrentTuple.cpp
    runtime/TrentBoolean.cpp
    runtime/TrentFloat.cpp

    PARENT_SCOPE
)