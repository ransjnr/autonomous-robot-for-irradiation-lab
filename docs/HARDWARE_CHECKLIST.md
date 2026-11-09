# Hardware handoff checklist

The supplied image is consistent with a generic 4WD Arduino arm kit, but it does **not** reveal its electronics. Photograph both sides of every board and read all labels before editing pin definitions.

## Identify first

| Part | Need to record | Why it matters |
| --- | --- | --- |
| Main controller | Arduino Uno/Nano, ESP32, Raspberry Pi, or other | Firmware toolchain and voltage levels differ. |
| Motor driver | L293D shield, L298N, TB6612FNG, or other | Pin and PWM mappings are driver-specific. |
| Motors / encoders | Voltage, encoder resolution, left/right polarity | Required for safe power and odometry. |
| Range sensor | 2D LiDAR preferred; depth camera minimum | Camera-only room mapping is not dependable on this chassis. |
| Camera | USB/CSI/IP and device path | Needed for barcode input. |
| Power | Battery voltage/current, buck converter, fuse and common ground | Prevents brownouts and controller damage. |

## Required safety sequence

1. Put the chassis on a stand: wheels off the floor and arm clear.
2. Test `PING`, then one motor at low PWM. Confirm `STOP` and the 350 ms command watchdog.
3. Test each servo individually with a limited range; mechanical end stops can destroy a servo.
4. Only after encoder and LiDAR topics are correct, run SLAM slowly in an open room.

## Architecture

`phone browser -> ROS /cmd_vel -> serial bridge -> ESP32 watchdog -> motor driver -> wheels`

`USB camera -> OpenCV barcode node -> /barcode_scan`

`encoders + LiDAR -> /odom + /scan + TF -> SLAM Toolbox -> /map`

The arm command is `/arm_command` with `[joint_index, angle_degrees]`; joint order is base, shoulder, elbow, gripper.
