add_executable(robot_control
    main.cpp
    tcp_client.cpp
    robot_client.cpp
)

target_include_directories(robot_control PRIVATE
    .
)

target_link_libraries(robot_control PRIVATE robot_core)
