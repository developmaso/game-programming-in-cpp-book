cmake_minimum_required(VERSION 3.10)

add_subdirectory(lib/googletest)

include(GoogleTest)
include(${PROJECT_SOURCE_DIR}/cmake/internal_utils.cmake)

function(add_unit_test name)
  set(TARGET_NAME ${name}Test)
  add_executable(${TARGET_NAME}
    ${name}Test.cpp
    ${PROJECT_SOURCE_DIR}/src/${name}.cpp
  )
  set_target_properties(${TARGET_NAME}
    PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
  )
  target_compile_options(${TARGET_NAME}
    PRIVATE -Wall -Wextra -Wno-long-long -pedantic
  )
  target_include_directories(${TARGET_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/src
  )
  target_link_libraries(${TARGET_NAME}
    gtest
    gtest_main
  )
  gtest_add_tests(
    TARGET ${TARGET_NAME}
  )
  clang_format(${TARGET_NAME})
endfunction()

set(TEST_SRCS
  ClassATest.cpp
  ClassBTest.cpp
)

foreach(loop_v IN LISTS TEST_SRCS)
  # "ClassATest.cpp" -> "ClassA"
  string(REGEX REPLACE Test\.cpp  "" test_name ${loop_v})
  add_unit_test(${test_name})
endforeach()
