cmake_minimum_required(VERSION 3.10)
project(flameshot-uploader LANGUAGES CXX VERSION 0.1.0) # Set languages to "C CXX" if using resources with gtkmm

# Source and build directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

# Find libraries
find_package(PkgConfig REQUIRED)
pkg_search_module(CURLPP REQUIRED curlpp)
# pkg_search_module(GTKMM REQUIRED gtkmm-3.0)
find_package(nlohmann_json 3.10 REQUIRED)

# Setup clip library
set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")
# Uncomment to enable png support for clip library
#set(CLIP_X11_PNG_LIBRARY "${PNG_LIBRARY}")
# Add clip library
add_subdirectory(libs/clip)

# Find all source and header files in src directory using file globbing
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.hpp") # "${SRC_DIR}/resources.c" "${SRC_DIR}/resources.h"

# Add source files to executable
add_executable(flameshot-uploader ${SRC_FILES})

# Set build output
set(EXECUTABLE_OUTPUT_PATH "${BUILD_DIR}")

# Link found libraries
target_include_directories(flameshot-uploader PRIVATE ${CURLPP_INCLUDE_DIRS}) # ${GTKMM_INCLUDE_DIRS}
target_link_libraries(flameshot-uploader PRIVATE ${CURLPP_LIBRARIES} nlohmann_json::nlohmann_json clip) # ${GTKMM_LIBRARIES}

# Set build flags
# Debug
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
# Release
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s")

# Specify c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use /usr instead of /usr/local; lintian complains about the unusual install location
set(CMAKE_INSTALL_PREFIX /usr)

install(
    TARGETS flameshot-uploader
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# DEB packaging
set(CPACK_PACKAGE_DIRECTORY "${BUILD_DIR}")
SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET(CPACK_SET_DESTDIR TRUE)
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
include(GNUInstallDirs)
set(CPACK_DEBIAN_FILE_NAME "flameshot-uploader-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}.deb")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "nlohmann-json3-dev (>= 3.10), libcurlpp-dev(>= 0.8), libc6-dev | libc-dev")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "emmaexe <emma@emmaexe.moe>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A C++ app expanding flameshot's screenshot uploading capabilities.")
set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/emmaexe/flameshot-uploader")
set(CPACK_DEBIAN_PACKAGE_ENHANCES "flameshot")
set(CPACK_COMPONENTS_ALL ON)
include(CPack)
