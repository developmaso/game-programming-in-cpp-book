find_package(GTest 1.8 REQUIRED)

function(add_unit_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name}
    GTest::GTest
    GTest::Main
    mylib
    )
  add_test(
    NAME ${name}
    COMMAND ${name}
    )
endfunction()

add_unit_test(ClassATest)
add_unit_test(ClassBTest)

#${PROJECT_SOURCE_DIR}/
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
