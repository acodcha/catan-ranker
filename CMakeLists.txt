cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

# Define the C++ build settings.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math -O3 -Wall -Wno-return-type")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Release)

# Define the CMake project.
project("catan-ranker" VERSION 1.0.0 LANGUAGES CXX)
option(BUILD_DOCS "Build the documentation using Doxygen." OFF)
option(BUILD_TESTING "Build the tests." ON)

# Build the executable.
set(EXECUTABLE_NAME "catan-ranker")
file(GLOB_RECURSE SOURCE_CPP source/*.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCE_CPP})
target_link_libraries(${EXECUTABLE_NAME} stdc++fs)

# Install the executable.
install(TARGETS ${EXECUTABLE_NAME} DESTINATION /usr/local/bin)

# Build the tests.
include(CTest)
if(BUILD_TESTING)
  enable_testing()
  add_test(NAME main_test COMMAND ../test/run.sh)
endif()

# Build the documentation.
if(BUILD_DOCS)
  find_package(Doxygen REQUIRED dot)
  if(NOT Doxygen_FOUND)
    message(WARNING "Cannot build the documentation because Doxygen is not installed. Install it with \"sudo apt install doxygen graphviz texlive texlive-fonts-extra\".")
    return()
  endif()
  add_custom_target(
    docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
    COMMENT "Building the documentation using Doxygen..."
    VERBATIM)
endif()
