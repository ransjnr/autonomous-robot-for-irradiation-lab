"""Bridge ROS movement commands to the ESP32 line protocol."""
import json
import serial
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from std_msgs.msg import Int16MultiArray, String

class SerialBridge(Node):
    def __init__(self):
        super().__init__('serial_bridge')
        self.declare_parameter('port', '/dev/ttyUSB0'); self.declare_parameter('baud', 115200)
        self.serial = serial.Serial(self.get_parameter('port').value, self.get_parameter('baud').value, timeout=.1)
        self.telemetry = self.create_publisher(String, '/robot/telemetry', 10)
        self.create_subscription(Twist, '/cmd_vel', self.drive, 10)
        self.create_subscription(Int16MultiArray, '/arm_command', self.arm, 10)
        self.create_timer(.1, self.read_status)
    def send(self, line): self.serial.write((line + '\n').encode())
    def drive(self, msg): self.send(f'DRIVE {max(-1,min(1,msg.linear.x)):.3f} {max(-1,min(1,msg.angular.z)):.3f}')
    def arm(self, msg):
        if len(msg.data) == 2: self.send(f'ARM {msg.data[0]} {msg.data[1]}')
    def read_status(self):
        line = self.serial.readline().decode(errors='replace').strip()
        if not line: return
        if line.startswith('{'):
            try: json.loads(line); self.telemetry.publish(String(data=line))
            except json.JSONDecodeError: self.get_logger().warning(f'Bad telemetry: {line}')
        else: self.get_logger().debug(line)
    def destroy_node(self):
        try: self.send('STOP'); self.serial.close()
        finally: super().destroy_node()
def main():
    rclpy.init(); n=SerialBridge()
    try: rclpy.spin(n)
    except KeyboardInterrupt: pass
    finally: n.destroy_node(); rclpy.shutdown()
