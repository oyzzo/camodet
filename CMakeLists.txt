cmake_minimum_required(VERSION 2.8)
project(camodet)
set(CMAKE_CXX_STANDARD 11)

function(add_clang_static_analysis target)
    get_target_property(SRCs ${target} SOURCES)
    add_library(${target}_analyze OBJECT EXCLUDE_FROM_ALL ${SRCs})
    set_target_properties(${target}_analyze PROPERTIES
                          COMPILE_OPTIONS "--analyze"
                          EXCLUDE_FROM_DEFAULT_BUILD true)
endfunction()

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( camodet camodet.cpp settings.cpp)
add_clang_static_analysis( camodet )
target_link_libraries( camodet ${OpenCV_LIBS} )


