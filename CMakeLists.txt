cmake_minimum_required(VERSION 3.10)
project(flameshot-uploader LANGUAGES C CXX VERSION 0.1.0)

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
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Specify gcc11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Custom target: make run
add_custom_target(run
    COMMAND "${BUILD_DIR}/flameshot-uploader"
    DEPENDS flameshot-uploader
    WORKING_DIRECTORY "${BUILD_DIR}"
)
add_custom_target(run_phony
    COMMAND "${CMAKE_COMMAND}" -E echo "Running flameshot-uploader..."
    DEPENDS run
)
