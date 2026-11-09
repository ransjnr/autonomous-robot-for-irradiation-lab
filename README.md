# Mobile Robot Stack

This is a separate, hardware-facing project for the small four-wheel, servo-arm style robot shown in the supplied photo. It deliberately does not modify the irradiation-lab simulation.

The picture is consistent with an Arduino Uno-class controller, a 4WD chassis (the two motors on each side are electrically paired), a battery holder, a 4-servo arm/gripper, and an add-on motor/sensor shield. Labels and wiring are not visible, so every pin definition is isolated in a configuration header. Never flash or apply power on the assumption that the example pins match the physical robot.

## Repository layout

```text
firmware/                         Microcontroller projects (flash one motor controller)
  arduino_uno_4wd_arm/            Uno: 4WD drive, arm, sonar, bumpers, encoders
  esp32_4wd_arm_controller/       ESP32 alternative motor controller
  esp32_camera_streamer/          ESP32-CAM image endpoint
ros2_ws/                          Robot-wide ROS 2 microservices
platforms/raspberry_pi/           Pi-specific provisioning and system service only
docs/                             Hardware, safety, architecture, and calibration guides
```

The `ros2_ws` is at the root because it is the robot's application code. It happens to run on the Raspberry Pi, but it is not Raspberry-Pi-specific.

## Included capabilities

- Arduino Uno firmware for the pictured kit class: paired-wheel drive, four servos, HC-SR04 ultrasonic obstacle reading, bumper input, battery reading, telemetry, watchdog stop, and configurable pins.
- ESP32 base firmware for the same low-level protocol, plus a separate ESP32-CAM MJPEG service for the camera board.
- ROS 2 microservices on a Raspberry Pi: serial bridge, safety gate, phone control, barcode/QR vision, and mapping.
- A phone-friendly local web controller that publishes `cmd_vel` through ROS 2.
- OpenCV barcode/QR detector that publishes JSON scan events.
- A SLAM Toolbox launch configuration for autonomous room mapping once the real robot provides `/scan`, `/odom`, and TF.

## Before connecting power

Read [`docs/HARDWARE_CHECKLIST.md`](docs/HARDWARE_CHECKLIST.md). Do not connect motors or servos to the controller's 5 V rail. Verify the driver type, motor voltage, common ground, wheel encoder wiring, and emergency stop first.

The full baseline component list and electrical limits are in [`hardware/README.md`](hardware/README.md). Follow [`docs/COMMISSIONING.md`](docs/COMMISSIONING.md) in order when the team has the hardware in front of them.

## Quick start

1. Identify every board and complete [`docs/HARDWARE_CHECKLIST.md`](docs/HARDWARE_CHECKLIST.md).
2. Flash the Uno: `cd firmware/arduino_uno_4wd_arm && pio run -t upload`. The Raspberry Pi connects to it over USB.
3. If the team replaces the Uno with an ESP32, use `firmware/esp32_4wd_arm_controller`; do not run both motor controllers at once.
4. Build the ROS workspace: `cd ros2_ws && colcon build --symlink-install`.
5. Run the bridge: `ros2 run mobile_robot_bringup serial_bridge --ros-args -p port:=/dev/ttyUSB0`.
5. Run phone control on the robot computer: `ros2 run mobile_robot_bringup phone_controller`; browse to `http://ROBOT_IP:8080` from the phone.
6. Run scanning: `ros2 run mobile_robot_bringup barcode_detector --ros-args -p camera_source:=0`; for an ESP32-CAM, use `-p camera_source:=http://ESP32_CAM_IP/capture`.

Before any actuator test, verify the USB connection only: `python3 platforms/raspberry_pi/tools/serial_smoke_test.py --port /dev/ttyACM0`.

Mapping is a separate safety-tested phase: `ros2 launch mobile_robot_bringup mapping.launch.py` after `/scan`, `/odom`, `base_link`, and `laser` are correct. An ultrasonic sensor is for emergency obstacle stopping, **not** room mapping.

Read the service contract in [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) before changing a component.

For Pi setup and autostart, use [`platforms/raspberry_pi`](platforms/raspberry_pi). It contains only Pi-specific setup and deployment material.

For autonomous operation, fit and calibrate encoders using [`docs/ENCODER_CALIBRATION.md`](docs/ENCODER_CALIBRATION.md). The Arduino profile leaves encoders disabled by default so it remains safe for the kit as photographed.
