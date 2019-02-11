cmake_minimum_required(VERSION 3.13)
project(cmake_googletest_sample CXX)
cmake_policy(SET CMP0076 NEW)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME} ${SRCS})
set_target_properties(${PROJECT_NAME}
  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)
target_compile_options(${PROJECT_NAME}
  PRIVATE -Wall -Wextra -Wno-long-long -pedantic
)
target_include_directories(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/src
)

enable_testing()
add_subdirectory(src)
add_subdirectory(test)
