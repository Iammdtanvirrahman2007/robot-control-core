#include "managed_robot.h"

ManagedRobot::ManagedRobot(std::string id, std::string host, uint16_t port)
    : client_(std::move(id)),
      host_(std::move(host)),
      port_(port),
      connected_(false) {}

bool ManagedRobot::connect() {
    connected_ = client_.connect_to(host_, port_);
    return connected_;
}

void ManagedRobot::disconnect() {
    connected_ = false;
}

bool ManagedRobot::connected() const {
    return connected_;
}

bool ManagedRobot::stop() {
    return client_.stop();
}

bool ManagedRobot::forward(uint8_t speed) {
    return client_.forward(speed);
}

bool ManagedRobot::backward(uint8_t speed) {
    return client_.backward(speed);
}

bool ManagedRobot::left(uint8_t speed) {
    return client_.left(speed);
}

bool ManagedRobot::right(uint8_t speed) {
    return client_.right(speed);
}

const std::string& ManagedRobot::id() const {
    return client_.id();
}
