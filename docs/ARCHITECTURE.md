# Microservice architecture

The robot uses small processes with explicit boundaries. A Raspberry Pi is the high-level computer; an Arduino Uno **or** ESP32 controls real-time outputs. Neither microcontroller is expected to perform SLAM, barcode recognition, or phone networking.

```text
phone browser -> phone_controller -> /cmd_vel/request -> safety_gate -> /cmd_vel -> serial_bridge -> Uno/ESP32
USB/ESP32-CAM -> barcode_detector -> /barcode_scan
Uno/ESP32 telemetry -> serial_bridge -> /robot/telemetry
encoders + LiDAR -> /odom + /scan + TF -> SLAM Toolbox -> /map -> Nav2 (only after validation)
```

## Services and contracts

| Service | Runs on | Input | Output | Failure behaviour |
| --- | --- | --- | --- | --- |
| `phone_controller` | Pi | HTTP on port 8080 | `/cmd_vel/request` | Stops publishing when the finger is released. |
| `safety_gate` | Pi | `/cmd_vel/request`, `/robot/telemetry` | `/cmd_vel` | Blocks movement when a bumper is pressed, ultrasonic clearance is too small, or commands go stale. |
| `serial_bridge` | Pi | `/cmd_vel`, `/arm_command` | `/robot/telemetry` | Sends `STOP` on shutdown; MCU watchdog stops on link loss. |
| `wheel_odometry` | Pi | `/robot/telemetry` ticks | `/odom`, `odom -> base_link` TF | Requires calibrated encoders; do not use for SLAM without them. |
| Uno or ESP32 firmware | MCU | serial lines | serial telemetry | Stops motors after 350 ms without command. |
| `barcode_detector` | Pi | camera device/stream | `/barcode_scan` | Never commands motors. |
| `slam_toolbox` | Pi | `/scan`, `/odom`, TF | `/map` | Not launched until sensor transforms are verified. |

## Serial protocol

Commands are newline-terminated UTF-8: `PING`, `STOP`, `DRIVE linear angular`, and `ARM joint angle`.

Telemetry is newline-delimited JSON: `{"range_cm":42.1,"bumper":0,"battery_v":7.31}`. Values must be treated as unavailable if they are absent or stale.

## Deployment rule

Only one process owns motor output: firmware receives its commands only from `serial_bridge`; only `safety_gate` is allowed to publish `/cmd_vel`. Camera and vision services have no movement authority.
