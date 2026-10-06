#pragma once

#include "robot_manager_bridge.h"

#include <cstdint>
#include <string>

class ControlHttpServer {
public:
    explicit ControlHttpServer(uint16_t port = 8080);

    void register_default_robots();
    void run();

private:
    uint16_t port_;
    RobotManagerBridge bridge_;

    std::string handle_request(const std::string& request);
    std::string json_response(bool ok, const std::string& message) const;
};
