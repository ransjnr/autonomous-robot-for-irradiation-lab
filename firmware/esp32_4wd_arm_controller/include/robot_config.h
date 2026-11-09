#pragma once

// CHANGE THESE AFTER YOU INSPECT THE REAL WIRING. This template targets an ESP32
// with a dual H-bridge driver (TB6612FNG/L298N-style direction+PWM inputs).
namespace cfg {
constexpr int LEFT_IN1 = 26, LEFT_IN2 = 27, LEFT_PWM = 25;
constexpr int RIGHT_IN1 = 32, RIGHT_IN2 = 33, RIGHT_PWM = 14;
constexpr int ARM_BASE_PIN = 13, ARM_SHOULDER_PIN = 19, ARM_ELBOW_PIN = 18, GRIPPER_PIN = 23;
constexpr int PWM_FREQ = 20000, PWM_BITS = 8;
constexpr unsigned long COMMAND_TIMEOUT_MS = 350;
constexpr int MAX_PWM = 210;  // start below 255 until the chassis is tested safely
}
