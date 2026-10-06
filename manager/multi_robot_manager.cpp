#include "multi_robot_manager.h"

ManagedRobot& MultiRobotManager::add_robot(
    const std::string& id,
    const std::string& host,
    uint16_t port) {

    auto robot = std::make_unique<ManagedRobot>(id, host, port);
    auto* result = robot.get();

    robots_[id] = std::move(robot);
    return *result;
}

ManagedRobot* MultiRobotManager::robot(const std::string& id) {
    auto it = robots_.find(id);
    return it == robots_.end() ? nullptr : it->second.get();
}

bool MultiRobotManager::connect_all() {
    bool all_connected = true;

    for (auto& [id, robot] : robots_) {
        if (!robot->connect()) {
            all_connected = false;
        }
    }

    return all_connected;
}

void MultiRobotManager::stop_all() {
    for (auto& [id, robot] : robots_) {
        robot->stop();
    }
}
