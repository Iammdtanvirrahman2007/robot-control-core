# C++ Host Controller

This is the first laptop-side control client.

It connects to an ESP32 over TCP and exposes a small RobotClient API:

    robot.forward(50)
    robot.backward(50)
    robot.left(40)
    robot.right(40)
    robot.stop()

Build from the repository root:

    cmake -S . -B build
    cmake --build build

Run:

    ./build/host/cpp/robot_control <ESP32_IP>

The ESP32 TCP endpoint must be listening on port 5000.

This client is intentionally small. Multi-robot management will be built on top of RobotClient next.
