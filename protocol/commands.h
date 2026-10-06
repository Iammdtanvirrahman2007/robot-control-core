#pragma once

#include <cstdint>

namespace robot_control {

enum class Command : uint8_t {
    Stop = 0,
    Forward,
    Backward,
    Left,
    Right
};

struct MotionCommand {
    Command command;
    uint8_t speed; // 0-100
};

} // namespace robot_control
