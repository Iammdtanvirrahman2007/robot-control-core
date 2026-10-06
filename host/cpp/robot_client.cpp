#include "robot_client.h"

#include <algorithm>
#include <string>

RobotClient::RobotClient(std::string robot_id)
    : robot_id_(std::move(robot_id)) {}

bool RobotClient::connect_to(const std::string& host, uint16_t port) {
    return tcp_.connect_to(host, port);
}

bool RobotClient::stop() {
    return send_motion("STOP", 0);
}

bool RobotClient::forward(uint8_t speed) {
    return send_motion("FORWARD", speed);
}

bool RobotClient::backward(uint8_t speed) {
    return send_motion("BACKWARD", speed);
}

bool RobotClient::left(uint8_t speed) {
    return send_motion("LEFT", speed);
}

bool RobotClient::right(uint8_t speed) {
    return send_motion("RIGHT", speed);
}

const std::string& RobotClient::id() const {
    return robot_id_;
}

bool RobotClient::send_motion(const char* command, uint8_t speed) {
    speed = std::min<uint8_t>(speed, 100);
    return tcp_.send_line(std::string(command) + " " + std::to_string(speed));
}
