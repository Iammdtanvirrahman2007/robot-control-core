#include "robot.h"

Robot::Robot(std::string id) : id_(std::move(id)) {}

const std::string& Robot::id() const {
    return id_;
}
