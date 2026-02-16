cmake_minimum_required( VERSION 3.10 )

project( hintro VERSION 0.1 LANGUAGES C )
set(PROJECT_DESCRIPTION "Random Synth Tone Generator Example using BASS library in C")

include(GNUInstallDirs)

if( NOT CMAKE_BUILD_TYPE )
        set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
endif()

set( CMAKE_C_STANDARD 23 )
set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_C_EXTENSIONS ON )

if( CMAKE_BUILD_TYPE EQUAL Debug )
	add_compile_options(-g)
endif()
if( CMAKE_GENERATOR_PLATFORM STREQUAL "x64" OR CMAKE_GENERATOR_PLATFORM STREQUAL "x86_64" OR CMAKE_GENERATOR_PLATFORM STREQUAL "AMD64" )
	add_compile_options(-mfpmath=sse)
endif()

add_compile_options(-march=native -O3)
add_compile_options(-fopenmp-simd)
add_compile_options(-ftree-vectorize)
add_compile_options(-Wno-gnu-alignof-expression)
add_compile_options(-D_FILE_OFFSET_BITS=64)
add_compile_options(-fdata-sections)
add_compile_options(-ffunction-sections)
add_compile_options(-Wno-array-bounds)
add_compile_options(-Wno-unused-parameter)
add_compile_options(-Wno-pedantic)
add_compile_options(-Wno-narrowing)
add_compile_options(-Wno-int-conversion)
add_link_options(-Wl,--gc-sections)
add_link_options(-Wl,--print-gc-sections)
add_link_options(-Wl,-s)

find_package(bass)

add_executable( ${PROJECT_NAME} hintro.c )
target_link_libraries( ${PROJECT_NAME} bass m )

