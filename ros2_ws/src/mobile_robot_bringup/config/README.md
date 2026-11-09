# Expected autonomous-navigation interfaces

The MCU firmware above is intentionally limited to low-level command execution. For safe mapping, add a robot-computer node that publishes wheel-encoder odometry and the TF transform `odom -> base_link`; use a LiDAR driver to publish `sensor_msgs/LaserScan` on `/scan` and `base_link -> laser`. SLAM Toolbox then builds `/map`. Nav2 must not be enabled until the robot has a physically verified costmap, bumper/emergency stop, and velocity limits.
