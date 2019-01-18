cmake_minimum_required(VERSION 3.4)

set(EXTDIR ${PROJECT_SOURCE_DIR}/externals)

add_executable(voroL5
  ${EXTDIR}/easylogging/src/easylogging++.cc
  main.cpp
  manager.cpp
  selftests.cpp
  selftest_main.cpp
)

target_include_directories(voroL5
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EXTDIR}/easylogging/src
    ${EXTDIR}/tclap/include
    ${Boost_INCLUDE_DIR}
    ${ARMADILLO_INCLUDE_DIRS}
)

target_link_libraries(voroL5
  Catch2::Catch2
  ${Boost_LIBRARIES}
  ${ARMADILLO_LIBRARY}
)
