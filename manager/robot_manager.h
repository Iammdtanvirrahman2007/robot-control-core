#pragma once

#include "robot.h"
#include <memory>
#include <string>
#include <unordered_map>

class RobotManager {
public:
    Robot& add_robot(const std::string& id);
    Robot* get_robot(const std::string& id);

private:
    std::unordered_map<std::string, std::unique_ptr<Robot>> robots_;
};
