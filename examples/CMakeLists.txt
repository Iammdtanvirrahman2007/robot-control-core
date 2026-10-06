add_executable(multi_robot_demo
    multi_robot.cpp
)

target_include_directories(multi_robot_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/manager
    ${CMAKE_SOURCE_DIR}/host/cpp
)

target_link_libraries(multi_robot_demo PRIVATE robot_core)
