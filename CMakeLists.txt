cmake_minimum_required(VERSION 3.13)

project(game_programming_in_cpp_book_chapter01 CXX)
set(PROJECT_VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# The target_sources() command converts relative paths to absolute.
cmake_policy(SET CMP0076 NEW)

add_executable(${PROJECT_NAME})
target_compile_options(${PROJECT_NAME}
  PRIVATE
  -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic
  -Wno-exit-time-destructors -Wno-implicit-fallthrough -Wno-missing-braces
  -Wno-padded
)
target_include_directories(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(${PROJECT_NAME} SYSTEM
  PRIVATE ${PROJECT_SOURCE_DIR}/external/SDL/include
)
target_link_directories(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/external/SDL/lib/mac
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  SDL2-2.0.0
)

enable_testing()
add_subdirectory(src)
# add_subdirectory(test)

include(cmake/internal_utils.cmake)
clang_format(${PROJECT_NAME})
