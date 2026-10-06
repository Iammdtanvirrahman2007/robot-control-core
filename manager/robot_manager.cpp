#include "robot_manager.h"

Robot& RobotManager::add_robot(const std::string& id) {
    auto [it, inserted] = robots_.try_emplace(id, std::make_unique<Robot>(id));
    return *it->second;
}

Robot* RobotManager::get_robot(const std::string& id) {
    auto it = robots_.find(id);
    return it == robots_.end() ? nullptr : it->second.get();
}
