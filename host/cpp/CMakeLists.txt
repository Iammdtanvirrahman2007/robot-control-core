add_executable(robot_control
    main.cpp
    tcp_client.cpp
    robot_client.cpp
)

target_include_directories(robot_control PRIVATE
    .
)

target_link_libraries(robot_control PRIVATE robot_core)

add_executable(robot_control_server
    control_server_main.cpp
    control_http_server.cpp
    robot_manager_bridge.cpp
)

target_include_directories(robot_control_server PRIVATE
    .
    ${CMAKE_SOURCE_DIR}/manager
    ${CMAKE_SOURCE_DIR}/protocol
)

target_link_libraries(robot_control_server PRIVATE robot_core)
