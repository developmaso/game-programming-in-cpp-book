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
  target_compile_options(${TARGET_NAME}
    PRIVATE -Werror -Wall -Wextra
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

set(SRCS
  ClassATest.cpp
  ClassBTest.cpp
)

foreach(NAME IN LISTS SRCS)
  # "ClassATest.cpp" -> "ClassA"
  string(REGEX REPLACE Test\.cpp  "" test_name ${NAME})
  add_unit_test(${test_name})
endforeach()
