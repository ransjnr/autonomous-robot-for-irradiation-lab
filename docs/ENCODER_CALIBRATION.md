# Wheel encoder and odometry calibration

The pictured robot may not have encoders. Add one encoder per wheel side (or mechanically couple both wheels on a side) before expecting autonomous mapping.

1. Set `ENCODERS_INSTALLED=true` only after wiring left and right encoder outputs to Uno D2 and D3.
2. Raise the chassis, check that ticks increase for a forward command and decrease in reverse. Swap motor polarity or adjust the sign logic if not.
3. Mark the wheel, drive exactly 1 m on the floor, and set `ticks_per_revolution` and `wheel_radius_m` in the `wheel_odometry` parameters until odometry measures 1 m.
4. Rotate in place one full turn and calibrate `wheel_base_m` until yaw is `2π` radians.
5. Add a LiDAR and validate the `laser -> base_link` transform before launching SLAM.

Single-channel encoders infer direction from the command and cannot detect slipping or pushed motion. Quadrature encoders are preferable for a production build.
