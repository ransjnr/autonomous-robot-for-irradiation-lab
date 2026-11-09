#pragma once

// Generic Uno + L298N/TB6612-style driver profile. This is NOT pinout evidence
// for the pictured robot: inspect its shield before connecting power.
namespace cfg {
constexpr byte LEFT_IN1=7, LEFT_IN2=8, LEFT_PWM=10;
constexpr byte RIGHT_IN1=12, RIGHT_IN2=13, RIGHT_PWM=11;
constexpr byte SERVO_BASE=4, SERVO_SHOULDER=5, SERVO_ELBOW=6, SERVO_GRIPPER=9;
constexpr byte ULTRASONIC_TRIG=A0, ULTRASONIC_ECHO=A1, BUMPER_PIN=A2, BATTERY_PIN=A3;
// Optional single-channel encoders use the Uno's two external interrupts.
// They are disabled until installed; count direction is inferred from commanded motor direction.
constexpr bool ENCODERS_INSTALLED=false;
constexpr byte LEFT_ENCODER_PIN=2, RIGHT_ENCODER_PIN=3;
constexpr int MAX_PWM=180;              // begin conservatively
constexpr unsigned long COMMAND_TIMEOUT_MS=350;
constexpr unsigned long TELEMETRY_PERIOD_MS=200;
constexpr float BATTERY_SCALE=2.0f;     // set after measuring the resistor divider
constexpr float STOP_DISTANCE_CM=12.0f; // firmware blocks forward motion inside this range
}
