#pragma once

#include "managed_robot.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

class MultiRobotManager {
public:
    ManagedRobot& add_robot(const std::string& id,
                            const std::string& host,
                            uint16_t port = 5000);

    ManagedRobot* robot(const std::string& id);

    bool connect_all();
    void stop_all();

private:
    std::unordered_map<std::string, std::unique_ptr<ManagedRobot>> robots_;
};
