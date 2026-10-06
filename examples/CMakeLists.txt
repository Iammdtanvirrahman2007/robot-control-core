add_executable(multi_robot_demo
    multi_robot.cpp
)

target_link_libraries(multi_robot_demo PRIVATE robot_core)
target_include_directories(multi_robot_demo PRIVATE
    ../manager
    ../host/cpp
)
