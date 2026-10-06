#include "robot_manager_bridge.h"

RobotManagerBridge::RobotManagerBridge() = default;

bool RobotManagerBridge::register_robot(
    const std::string& id,
    const std::string& host,
    uint16_t port) {
    manager_.add_robot(id, host, port);
    return true;
}

bool RobotManagerBridge::connect_robot(const std::string& id) {
    auto* robot = manager_.robot(id);
    return robot != nullptr && robot->connect();
}

bool RobotManagerBridge::command(
    const std::string& id,
    const std::string& action,
    uint8_t speed) {

    auto* robot = manager_.robot(id);
    if (!robot) return false;

    if (!robot->connected() && !robot->connect()) {
        return false;
    }

    if (action == "STOP") return robot->stop();
    if (action == "FORWARD") return robot->forward(speed);
    if (action == "BACKWARD") return robot->backward(speed);
    if (action == "LEFT") return robot->left(speed);
    if (action == "RIGHT") return robot->right(speed);

    return false;
}

void RobotManagerBridge::emergency_stop_all() {
    manager_.stop_all();
}
