# Arduino Uno wiring: paired 4WD + L298N/TB6612 profile

This is the exact profile implemented in `firmware/arduino_uno_4wd_arm/include/robot_config.h`. It applies only to a direct-input L298N or TB6612-style driver. It does **not** apply to an L293D shield that plugs directly onto an Uno; identify that shield first.

| Arduino Uno pin | Connect to | Notes |
| --- | --- | --- |
| D7, D8, D10 PWM | left IN1, IN2, enable/PWMA | Both left motors are parallel on the left channel. |
| D12, D13, D11 PWM | right IN1, IN2, enable/PWMB | Both right motors are parallel on the right channel. |
| D4, D5, D6, D9 | base, shoulder, elbow, gripper servo signals | Servo red wires go to a separate 5–6 V supply. |
| A0 / A1 | HC-SR04 TRIG / ECHO | Uno accepts the 5 V echo signal. |
| A2 | normally-open bumper switch to ground | Internal pull-up enabled; closed bumper reads active. |
| A3 | battery divider midpoint | Use high-value resistor divider; calibrate `BATTERY_SCALE`. |
| D2 / D3 | left / right encoder signal | Optional; enable only after fitting encoders. |
| USB | Raspberry Pi | USB serial at 115200 baud. |

## Power topology

```text
battery + -> fuse -> motor-driver VM
                   -> 5–6 V / 5 A buck -> servos
                   -> regulated 5 V -> Uno and Pi (use a Pi-rated supply)
battery - --------------------------------> common ground
```

Do not draw servo or motor current from Uno 5 V, Pi USB, or the motor driver logic regulator.
