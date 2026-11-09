#include <Arduino.h>
#include <Servo.h>
#include "robot_config.h"

Servo joints[4];
unsigned long last_command = 0, last_telemetry = 0;
float range_cm = -1;
volatile long left_ticks=0, right_ticks=0;
volatile int left_sign=1, right_sign=1;
void on_left_tick(){ left_ticks+=left_sign; }
void on_right_tick(){ right_ticks+=right_sign; }

void set_motor(byte in1, byte in2, byte pwm, int value) {
  value = constrain(value, -cfg::MAX_PWM, cfg::MAX_PWM);
  digitalWrite(in1, value > 0); digitalWrite(in2, value < 0); analogWrite(pwm, abs(value));
}
void stop_motors() { set_motor(cfg::LEFT_IN1,cfg::LEFT_IN2,cfg::LEFT_PWM,0); set_motor(cfg::RIGHT_IN1,cfg::RIGHT_IN2,cfg::RIGHT_PWM,0); }
float ultrasonic_cm() {
  digitalWrite(cfg::ULTRASONIC_TRIG,LOW); delayMicroseconds(2); digitalWrite(cfg::ULTRASONIC_TRIG,HIGH); delayMicroseconds(10); digitalWrite(cfg::ULTRASONIC_TRIG,LOW);
  unsigned long us=pulseIn(cfg::ULTRASONIC_ECHO,HIGH,25000); return us ? us/58.0f : -1;
}
void drive(float linear, float angular) {
  // Protect only forward travel; reverse is retained as an escape manoeuvre.
  if (linear > 0 && range_cm > 0 && range_cm < cfg::STOP_DISTANCE_CM) linear=0;
  int left=255*constrain(linear-angular,-1.0f,1.0f), right=255*constrain(linear+angular,-1.0f,1.0f);
  left_sign=left>=0?1:-1; right_sign=right>=0?1:-1;
  set_motor(cfg::LEFT_IN1,cfg::LEFT_IN2,cfg::LEFT_PWM,left); set_motor(cfg::RIGHT_IN1,cfg::RIGHT_IN2,cfg::RIGHT_PWM,right);
}
void command(String line) {
  line.trim(); last_command=millis();
  if(line=="PING") { Serial.println(F("PONG")); return; }
  if(line=="STOP") { stop_motors(); Serial.println(F("OK STOP")); return; }
  float linear,angular; int joint,angle;
  if(sscanf(line.c_str(),"DRIVE %f %f",&linear,&angular)==2) { drive(linear,angular); Serial.println(F("OK DRIVE")); return; }
  if(sscanf(line.c_str(),"ARM %d %d",&joint,&angle)==2 && joint>=0 && joint<4) { joints[joint].write(constrain(angle,0,180)); Serial.println(F("OK ARM")); }
}
void telemetry() {
  range_cm=ultrasonic_cm(); bool bumper=!digitalRead(cfg::BUMPER_PIN);
  float volts=analogRead(cfg::BATTERY_PIN)*5.0f/1023.0f*cfg::BATTERY_SCALE;
  noInterrupts(); long l=left_ticks, r=right_ticks; interrupts();
  Serial.print(F("{\"range_cm\":")); Serial.print(range_cm,1); Serial.print(F(",\"bumper\":")); Serial.print(bumper?1:0); Serial.print(F(",\"battery_v\":")); Serial.print(volts,2); Serial.print(F(",\"left_ticks\":")); Serial.print(l); Serial.print(F(",\"right_ticks\":")); Serial.print(r); Serial.println(F("}"));
}
void setup() {
  Serial.begin(115200);
  byte outputs[]={cfg::LEFT_IN1,cfg::LEFT_IN2,cfg::LEFT_PWM,cfg::RIGHT_IN1,cfg::RIGHT_IN2,cfg::RIGHT_PWM,cfg::ULTRASONIC_TRIG};
  for(byte p:outputs) pinMode(p,OUTPUT);
  pinMode(cfg::ULTRASONIC_ECHO,INPUT); pinMode(cfg::BUMPER_PIN,INPUT_PULLUP); stop_motors();
  if(cfg::ENCODERS_INSTALLED) { pinMode(cfg::LEFT_ENCODER_PIN,INPUT_PULLUP); pinMode(cfg::RIGHT_ENCODER_PIN,INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(cfg::LEFT_ENCODER_PIN),on_left_tick,RISING); attachInterrupt(digitalPinToInterrupt(cfg::RIGHT_ENCODER_PIN),on_right_tick,RISING); }
  byte pins[]={cfg::SERVO_BASE,cfg::SERVO_SHOULDER,cfg::SERVO_ELBOW,cfg::SERVO_GRIPPER}; for(byte i=0;i<4;i++){joints[i].attach(pins[i]);joints[i].write(90);}
  last_command=millis(); Serial.println(F("READY uno_robot"));
}
void loop() {
  if(Serial.available()) command(Serial.readStringUntil('\n'));
  if(millis()-last_command>cfg::COMMAND_TIMEOUT_MS) stop_motors();
  if(millis()-last_telemetry>cfg::TELEMETRY_PERIOD_MS){last_telemetry=millis();telemetry();}
}
