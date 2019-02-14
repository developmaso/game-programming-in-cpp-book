cmake_minimum_required(VERSION 3.10)

add_subdirectory(lib/googletest)

include(GoogleTest)
include(${PROJECT_SOURCE_DIR}/cmake/internal_utils.cmake)

function(add_tests name)
  set(TARGET_NAME ${name}_test)
  add_executable(${TARGET_NAME}
    ${TARGET_NAME}.cpp
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
  class_a_test.cpp
  class_b_test.cpp
)

foreach(NAME IN LISTS SRCS)
  # "class_a_test.cpp" -> "class_a"
  string(REGEX REPLACE _test\.cpp  "" test_name ${NAME})
  add_tests(${test_name})
endforeach()
