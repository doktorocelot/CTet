cmake_minimum_required(VERSION 3.25)

project(CTetDirectX CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        include/ctetdx.h
        directx-renderer.cpp
)


add_library(CTetDirectX STATIC ${SOURCES})
target_include_directories(CTetDirectX PUBLIC include/)
target_link_libraries(CTetDirectX PUBLIC d3d11.lib dxguid.lib)