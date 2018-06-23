cmake_minimum_required(VERSION 2.8)

project(camodet)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

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

#CPackage section
INCLUDE(InstallRequiredSystemLibraries)
INSTALL(TARGETS camodet DESTINATION bin)

#Set to the desired: DEB, STGZ, RPM, NSIS, ETC...
#SET(CPACK_GENERATOR "DEB") #if none set, the default applies
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Noel Carriquí") #required

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CAmera MOtion DETection")
SET(CPACK_PACKAGE_VENDOR "Noel Carriquí")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")

IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  # SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "camodet.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} CAmera MOtion DETection")
  SET(CPACK_NSIS_CONTACT "noel.carriqui@gmail.com")
  SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_STRIP_FILES "camodet")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)

INCLUDE(CPack)
