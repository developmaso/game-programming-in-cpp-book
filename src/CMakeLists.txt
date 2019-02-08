add_executable(${PROJECT_NAME}
  ClassA.cpp
  ClassB.cpp
  main.cpp
  )
target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/src
  )
