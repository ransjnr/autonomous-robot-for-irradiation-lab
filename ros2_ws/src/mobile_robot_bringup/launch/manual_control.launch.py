from launch import LaunchDescription
from launch_ros.actions import Node
def generate_launch_description():
    return LaunchDescription([
        Node(package='mobile_robot_bringup', executable='serial_bridge', parameters=[{'port':'/dev/ttyACM0'}], output='screen'),
        Node(package='mobile_robot_bringup', executable='safety_gate', output='screen'),
        Node(package='mobile_robot_bringup', executable='wheel_odometry', output='screen'),
        Node(package='mobile_robot_bringup', executable='phone_controller', output='screen'),
    ])
