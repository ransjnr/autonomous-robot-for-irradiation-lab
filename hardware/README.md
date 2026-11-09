# Hardware baseline from the supplied image

The pictured platform appears to be a generic 4WD acrylic robot with a 4DOF servo arm/gripper, an Arduino Uno-class board, a 2-cell 18650 holder, four yellow TT gearmotors, and an add-on shield. It may have been sold with an L293D motor shield, an L298N board, or a similar controller; the photo is insufficient to distinguish them.

Use the wiring sheet only after matching the physical motor driver. Do not connect a guessed pinout.

## Required build list

| Role | Minimum suitable component | Purpose |
| --- | --- | --- |
| Low-level controller | Arduino Uno **or** ESP32 | Motor, servo, bumper, ultrasonic control. |
| Motor driver | TB6612FNG preferred; L298N acceptable | Drives paired left and right motors. |
| Motor power | Fused battery feed rated for stall current | Motors must not be powered through Arduino/Pi 5 V. |
| Servo power | 5–6 V buck converter, 5 A or higher | Separately powers all arm servos. |
| Compute | Raspberry Pi 4/5 + USB storage | ROS 2, OpenCV, mapping. |
| Safety obstacle sensor | HC-SR04 or better, plus physical bumper | Near-field stop only. |
| Mapping sensor | 2D LiDAR with ROS 2 driver | Required for useful room mapping. |
| Odometry | Two wheel encoders minimum | Required for stable SLAM. |
| Vision | Pi USB camera or ESP32-CAM | Barcode/QR detection. |

## Electrical non-negotiables

1. Fuse the battery near its positive terminal.
2. Join grounds of battery buck converter, motor driver, MCU, and Pi **at one common reference**.
3. Use a dedicated buck converter for servos. A brownout while the arm moves will reset the controller.
4. Level-shift or divide the HC-SR04 ECHO output to 3.3 V for an ESP32. It is not safe to feed 5 V into an ESP32 GPIO.
5. Unplug motor power during first firmware flash and test each actuator with wheels raised.

See [`wiring/uno_l298n_or_tb6612.md`](wiring/uno_l298n_or_tb6612.md) for the profile implemented by the Arduino firmware.
