#pragma once

#include "multi_robot_manager.h"

#include <cstdint>
#include <string>

class RobotManagerBridge {
public:
    RobotManagerBridge();

    bool register_robot(const std::string& id,
                        const std::string& host,
                        uint16_t port = 5000);

    bool connect_robot(const std::string& id);
    bool command(const std::string& id,
                 const std::string& action,
                 uint8_t speed);

    void emergency_stop_all();

private:
    MultiRobotManager manager_;
};
