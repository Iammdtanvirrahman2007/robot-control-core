#pragma once

#include "../host/cpp/robot_client.h"

#include <cstdint>
#include <string>

class ManagedRobot {
public:
    ManagedRobot(std::string id, std::string host, uint16_t port = 5000);

    bool connect();
    void disconnect();

    bool connected() const;

    bool stop();
    bool forward(uint8_t speed);
    bool backward(uint8_t speed);
    bool left(uint8_t speed);
    bool right(uint8_t speed);

    const std::string& id() const;

private:
    RobotClient client_;
    std::string host_;
    uint16_t port_;
    bool connected_;
};
