#include "robot_client.h"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: robot_control <ESP32_IP>\n";
        return 1;
    }

    RobotClient robot("R1");

    if (!robot.connect_to(argv[1], 5000)) {
        std::cerr << "Could not connect to robot at " << argv[1] << "\n";
        return 1;
    }

    std::cout << "Connected to " << robot.id() << "\n";
    std::cout << "Commands: w=forward, s=backward, a=left, d=right, x=stop, q=quit\n";

    char key;
    while (std::cin >> key) {
        bool ok = true;

        switch (key) {
            case 'w': ok = robot.forward(50); break;
            case 's': ok = robot.backward(50); break;
            case 'a': ok = robot.left(40); break;
            case 'd': ok = robot.right(40); break;
            case 'x': ok = robot.stop(); break;
            case 'q': robot.stop(); return 0;
            default:
                std::cout << "Unknown command\n";
                continue;
        }

        std::cout << (ok ? "sent\n" : "send failed\n");
    }

    return 0;
}
