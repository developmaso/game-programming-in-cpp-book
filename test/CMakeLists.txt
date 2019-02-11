cmake_minimum_required(VERSION 3.10)

add_subdirectory(lib/googletest)

include(GoogleTest)

function(add_unit_test name)
  add_executable(${name}Test
    ${name}Test.cpp
    ${PROJECT_SOURCE_DIR}/src/${name}.cpp
  )
  set_target_properties(${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
  )
  target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -Wextra -Wno-long-long -pedantic
  )
  target_include_directories(${name}Test
    PRIVATE ${PROJECT_SOURCE_DIR}/src
  )
  target_link_libraries(${name}Test
    gtest
    gtest_main
  )
  gtest_add_tests(
    TARGET ${name}Test
  )
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
