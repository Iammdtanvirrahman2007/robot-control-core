#pragma once

// Change these pins to match your motor driver wiring.
namespace robot_config {

constexpr int LEFT_IN1  = 26;
constexpr int LEFT_IN2  = 27;
constexpr int LEFT_PWM  = 25;

constexpr int RIGHT_IN1 = 32;
constexpr int RIGHT_IN2 = 33;
constexpr int RIGHT_PWM = 14;

constexpr int PWM_FREQUENCY = 1000;
constexpr int PWM_RESOLUTION = 8;

constexpr unsigned long COMMAND_TIMEOUT_MS = 2000;

constexpr const char* ROBOT_ID = "R1";

} // namespace robot_config
