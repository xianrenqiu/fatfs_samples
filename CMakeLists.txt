# CMake minimum version
cmake_minimum_required(VERSION 3.8)

# Project Infomation
project(fatfs C)

# Debug
set(CMAKE_BUILD_TYPE "Debug")

# CFLAGS
set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -w -g -ggdb")

# Reset output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/library)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/library)

# fatfs library
set(fatfs_dir ${PROJECT_SOURCE_DIR}/source)
include_directories(${fatfs_dir})
aux_source_directory(${fatfs_dir} fatfs_source)

add_library(fatfs SHARED ${fatfs_source})
add_library(fatfs_static STATIC ${fatfs_source})
set_target_properties(fatfs_static PROPERTIES OUTPUT_NAME "fatfs")

# samples
add_executable(samples ${PROJECT_SOURCE_DIR}/samples.c) 
target_link_libraries(samples fatfs)
