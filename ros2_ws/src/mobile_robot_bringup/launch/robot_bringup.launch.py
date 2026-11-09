"""Starts manual driving, hardware bridge, safety gate, and wheel odometry."""
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
import os
def generate_launch_description():
    params=os.path.join(get_package_share_directory('mobile_robot_bringup'),'config','robot.yaml')
    return LaunchDescription([
        Node(package='mobile_robot_bringup',executable='serial_bridge',parameters=[params],output='screen'),
        Node(package='mobile_robot_bringup',executable='safety_gate',parameters=[params],output='screen'),
        Node(package='mobile_robot_bringup',executable='wheel_odometry',parameters=[params],output='screen'),
        Node(package='mobile_robot_bringup',executable='phone_controller',output='screen'),
    ])
