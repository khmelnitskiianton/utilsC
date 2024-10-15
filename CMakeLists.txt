#------------------------------------------------------------------------------
#
# CMake approach
# * dependencies are all correct
# * additional library output
# * object targets and separate dir for free
#
#------------------------------------------------------------------------------
#
# This file is licensed after LGPL v3
# Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
#
#------------------------------------------------------------------------------

# Commands
#
# rm -rf build & mkdir build
# cmake -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Release -S . -B build 
# cmake --build ./build
# env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test

cmake_minimum_required(VERSION 3.11)

project("MyProject")

enable_testing()

set(PREF_SRC "./src")
set(PREF_HDR "./include")

include_directories(PREF_HDR) 
 
FILE(GLOB SRC ${PREF_SRC}/*.cpp)
FILE(GLOB HDR ${PREF_HDR}/*.h)

set(NAME_EXE "program")
add_executable(${NAME_EXE} ${SRC} ${HDR})

# Tests

file(GLOB testfiles "Tests/*.dat")
foreach(file ${testfiles})
  add_test(
    NAME ${file}
    COMMAND bash -c "${CMAKE_SOURCE_DIR}/runtest.sh ${file} ${NAME_EXE}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties(${file} PROPERTIES DEPENDS ${NAME_EXE})
endforeach()

#------------------------------------------------------------------------------
# SET(SRCS
#   source/list.c
#   source/cache.c
#   source/hash.c
# )
# 
# SET(TESTS ${CMAKE_SOURCE_DIR}/Tests)
# 
# add_library(lclib STATIC ${SRCS})
# target_include_directories(lclib PUBLIC "include")
# 
# add_executable(LC.x LCmain.c)
# target_link_libraries(LC.x lclib)
#------------------------------------------------------------------------------
# message(STATUS "MYVAR = ${MYVAR}")
# -DCMAKE_C_FLAGS 
# -G "Unix Makefiles"