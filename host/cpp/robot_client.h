#pragma once

#include "tcp_client.h"
#include <cstdint>
#include <string>

class RobotClient {
public:
    explicit RobotClient(std::string robot_id);

    bool connect_to(const std::string& host, uint16_t port);

    bool stop();
    bool forward(uint8_t speed);
    bool backward(uint8_t speed);
    bool left(uint8_t speed);
    bool right(uint8_t speed);

    const std::string& id() const;

private:
    bool send_motion(const char* command, uint8_t speed);

    std::string robot_id_;
    TcpClient tcp_;
};
