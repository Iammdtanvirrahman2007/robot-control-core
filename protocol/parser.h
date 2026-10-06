#pragma once

#include "commands.h"

#include <optional>
#include <string>

namespace robot_control {

std::optional<MotionCommand> parse_command(const std::string& line);
const char* command_name(Command command);
std::string format_command(const MotionCommand& command);

} // namespace robot_control
