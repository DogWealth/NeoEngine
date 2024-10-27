cmake_minimum_required(VERSION 3.29)
project(NeoEngine)

set(CMAKE_CXX_STANDARD 20)

include_directories(NeoEngine/Engine/src)

add_subdirectory(NeoEngine)
add_subdirectory(TestBed)

file(COPY ${CMAKE_BINARY_DIR}/NeoEngine/Engine/libNeoEngine.dll DESTINATION ${CMAKE_BINARY_DIR}/TestBed)