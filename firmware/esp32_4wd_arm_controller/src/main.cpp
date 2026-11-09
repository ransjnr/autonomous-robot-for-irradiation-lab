#include <Arduino.h>
#include <ESP32Servo.h>
#include "robot_config.h"

Servo arm[4];
unsigned long last_command_ms = 0;

void motor(int in1, int in2, int pwm_pin, int value) {
  value = constrain(value, -cfg::MAX_PWM, cfg::MAX_PWM);
  digitalWrite(in1, value > 0);
  digitalWrite(in2, value < 0);
  ledcWrite(pwm_pin, abs(value));
}
void stop() { motor(cfg::LEFT_IN1,cfg::LEFT_IN2,cfg::LEFT_PWM,0); motor(cfg::RIGHT_IN1,cfg::RIGHT_IN2,cfg::RIGHT_PWM,0); }
void drive(float linear, float angular) {
  // Input is normalized -1..1. Verify wheel orientation with the chassis raised.
  int left = int(255 * constrain(linear - angular, -1.0f, 1.0f));
  int right = int(255 * constrain(linear + angular, -1.0f, 1.0f));
  motor(cfg::LEFT_IN1,cfg::LEFT_IN2,cfg::LEFT_PWM,left);
  motor(cfg::RIGHT_IN1,cfg::RIGHT_IN2,cfg::RIGHT_PWM,right);
}
void handle(String s) {
  s.trim(); last_command_ms = millis();
  if (s == "STOP") { stop(); Serial.println("OK STOP"); return; }
  if (s.startsWith("DRIVE ")) { float v,w; if (sscanf(s.c_str(), "DRIVE %f %f", &v, &w) == 2) { drive(v,w); Serial.println("OK DRIVE"); } return; }
  if (s.startsWith("ARM ")) { int index,angle; if (sscanf(s.c_str(), "ARM %d %d", &index,&angle) == 2 && index >= 0 && index < 4) { arm[index].write(constrain(angle,0,180)); Serial.println("OK ARM"); } return; }
  if (s == "PING") Serial.println("PONG");
}
void setup() {
  Serial.begin(115200);
  for (int p : {cfg::LEFT_IN1,cfg::LEFT_IN2,cfg::RIGHT_IN1,cfg::RIGHT_IN2}) pinMode(p, OUTPUT);
  for (int p : {cfg::LEFT_PWM,cfg::RIGHT_PWM}) { ledcAttach(p,cfg::PWM_FREQ,cfg::PWM_BITS); ledcWrite(p,0); }
  int pins[] = {cfg::ARM_BASE_PIN,cfg::ARM_SHOULDER_PIN,cfg::ARM_ELBOW_PIN,cfg::GRIPPER_PIN};
  for (int i=0;i<4;i++) { arm[i].setPeriodHertz(50); arm[i].attach(pins[i],500,2400); arm[i].write(90); }
  last_command_ms = millis(); stop(); Serial.println("READY esp32_robot");
}
void loop() {
  if (Serial.available()) handle(Serial.readStringUntil('\n'));
  if (millis() - last_command_ms > cfg::COMMAND_TIMEOUT_MS) stop();
}
