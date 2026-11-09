#!/usr/bin/env bash
set -euo pipefail
# Run on Raspberry Pi OS/Ubuntu after ROS 2 is installed. Review before sudo use.
sudo apt-get update
sudo apt-get install -y python3-serial python3-opencv v4l-utils
echo 'Install ROS 2, slam_toolbox, Nav2, your LiDAR driver, and encoder driver for your distribution.'
echo 'Build: cd ~/mobile_robot_stack/ros2_ws && colcon build --symlink-install'
