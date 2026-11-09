# Commissioning sequence

Follow this order; do not skip ahead to autonomous mode.

1. **Inventory**: compare physical labels with `hardware/`; select Uno or ESP32 motor firmware, never both.
2. **Power**: inspect fuse, buck voltages, common ground, and servo supply with a multimeter before connecting controller inputs.
3. **Flash**: upload firmware with motor power disconnected. Connect the Pi by USB and run `PING` through the serial smoke test.
4. **Actuators**: support the chassis off the ground; test STOP, each wheel side at low PWM, and each servo at a limited range.
5. **Safety**: block ultrasonic sensor and press bumper; forward command must be rejected. Confirm watchdog stops motors after 350 ms.
6. **Odometry**: enable encoders, calibrate via `ENCODER_CALIBRATION.md`, and verify `/odom` while driving straight and rotating.
7. **Mapping**: mount LiDAR rigidly, validate `/scan` and TF, then run SLAM in a clear room at walking pace.
8. **Vision**: check barcode events while robot is stationary before integrating mission behaviour.

Any failed safety test is a stop condition, not something to bypass in software.
