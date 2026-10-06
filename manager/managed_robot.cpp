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
    client_.disconnect();
    connected_ = false;
}

bool ManagedRobot::connected() const {
    return connected_ && client_.connected();
}

bool ManagedRobot::stop() {
    const bool ok = client_.stop();
    if (!ok) connected_ = false;
    return ok;
}

bool ManagedRobot::forward(uint8_t speed) {
    const bool ok = client_.forward(speed);
    if (!ok) connected_ = false;
    return ok;
}

bool ManagedRobot::backward(uint8_t speed) {
    const bool ok = client_.backward(speed);
    if (!ok) connected_ = false;
    return ok;
}

bool ManagedRobot::left(uint8_t speed) {
    const bool ok = client_.left(speed);
    if (!ok) connected_ = false;
    return ok;
}

bool ManagedRobot::right(uint8_t speed) {
    const bool ok = client_.right(speed);
    if (!ok) connected_ = false;
    return ok;
}

const std::string& ManagedRobot::id() const {
    return client_.id();
}
