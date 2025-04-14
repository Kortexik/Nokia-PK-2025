cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
project(CppStdExample)

# Set boost to "/workspace/boost_1_86_0"
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR ROOT_PATH  ROOT_PATH)
set(BOOST_ROOT "${ROOT_PATH}workspace/boost_1_86_0")
# set(BOOST_ROOT "../../../boost_1_86_0")
message(STATUS "BOOST_ROOT: ${BOOST_ROOT}")
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

# Discover all cpp files in the directory
file(GLOB CPP_FILES CONFIGURE_DEPENDS "*.cpp")

# Create a separate target executable for each cpp file
foreach(CPP_FILE ${CPP_FILES})
    get_filename_component(TARGET_NAME ${CPP_FILE} NAME_WE)
    add_executable(${TARGET_NAME} ${CPP_FILE})
endforeach()
