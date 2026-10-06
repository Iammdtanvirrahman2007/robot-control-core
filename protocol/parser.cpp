#include "parser.h"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace robot_control {

namespace {

std::string upper(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return value;
}

} // namespace

std::optional<MotionCommand> parse_command(const std::string& line) {
    std::istringstream input(line);
    std::string action;
    int speed = 0;
    std::string extra;

    if (!(input >> action >> speed) || (input >> extra)) {
        return std::nullopt;
    }

    action = upper(action);
    if (speed < 0 || speed > 100) {
        return std::nullopt;
    }

    Command command;
    if (action == "STOP") command = Command::Stop;
    else if (action == "FORWARD") command = Command::Forward;
    else if (action == "BACKWARD") command = Command::Backward;
    else if (action == "LEFT") command = Command::Left;
    else if (action == "RIGHT") command = Command::Right;
    else return std::nullopt;

    if (command == Command::Stop && speed != 0) {
        return std::nullopt;
    }

    return MotionCommand{command, static_cast<uint8_t>(speed)};
}

const char* command_name(Command command) {
    switch (command) {
        case Command::Stop: return "STOP";
        case Command::Forward: return "FORWARD";
        case Command::Backward: return "BACKWARD";
        case Command::Left: return "LEFT";
        case Command::Right: return "RIGHT";
    }
    return "UNKNOWN";
}

std::string format_command(const MotionCommand& command) {
    return std::string(command_name(command.command)) + " " +
           std::to_string(command.speed);
}

} // namespace robot_control
