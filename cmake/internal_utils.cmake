# For using ClangFormat
option(ENABLE_CLANG_FORMAT
  "If the command clang-format is avilable, format source files before each build.\
Turn this off if the build time is too slow."
  ON
)
find_program(CLANG_FORMAT clang-format)

function(clang_format target)
  set(TARGET_NAME clang_format_${target})
  if(CLANG_FORMAT)
    get_target_property(SRCS ${target} SOURCES)
    add_custom_target(
      ${TARGET_NAME}
      COMMAND ${CLANG_FORMAT} -i -style=file ${SRCS}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    if(ENABLE_CLANG_FORMAT)
      message(STATUS "Enable ClangFormat for ${target}")
      add_dependencies(${target} ${TARGET_NAME})
    endif()
  endif()
endfunction()
