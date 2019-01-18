cmake_minimum_required(VERSION 3.4)

project(voloL5)

# Set path for CMake dependencies
IF(NOT DEFINED CMAKE_MODULE_PATH)
  # This block is executed only if this project is configuread as stand alone
  # that is not as a subproject of 'femdk' project
  SET(CMAKE_MODULE_PATH 
      "${CMAKE_SOURCE_DIR}/config/cmake_modules/" 
      CACHE PATH "Path to simple_odgen configuration directory")
ENDIF(NOT DEFINED CMAKE_MODULE_PATH)

set (CMAKE_CXX_STANDARD 11)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Armadillo)
find_package(Catch2 REQUIRED)
if (Catch2_FOUND)
  get_target_property(CID Catch2::Catch2 INTERFACE_INCLUDE_DIRECTORIES)
  message(STATUS "Catch2 header files: ${CID} ")
endif (Catch2_FOUND)

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)

enable_testing()

#add_subdirectory(docs)
add_subdirectory(src)
#add_subdirectory(tests)
