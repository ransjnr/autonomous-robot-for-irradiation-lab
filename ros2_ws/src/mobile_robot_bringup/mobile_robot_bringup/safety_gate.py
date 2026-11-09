"""The only ROS service permitted to forward human motion requests to hardware."""
import json
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from std_msgs.msg import String

class SafetyGate(Node):
    def __init__(self):
        super().__init__('safety_gate')
        self.declare_parameter('minimum_clearance_cm', 18.0)
        self.pub=self.create_publisher(Twist,'/cmd_vel',10)
        self.latest={}; self.create_subscription(Twist,'/cmd_vel/request',self.request,10); self.create_subscription(String,'/robot/telemetry',self.telemetry,10)
    def telemetry(self,msg):
        try: self.latest=json.loads(msg.data)
        except json.JSONDecodeError: self.get_logger().warning('Ignoring malformed telemetry')
    def request(self,msg):
        bumper=self.latest.get('bumper',1) # fail closed if telemetry absent
        clearance=self.latest.get('range_cm',-1)
        blocked=bumper or (msg.linear.x>0 and (clearance<0 or clearance<self.get_parameter('minimum_clearance_cm').value))
        if blocked:
            self.get_logger().warning('Motion blocked by safety telemetry'); self.pub.publish(Twist())
        else: self.pub.publish(msg)
def main():
    rclpy.init(); node=SafetyGate()
    try:rclpy.spin(node)
    except KeyboardInterrupt:pass
    finally:node.destroy_node();rclpy.shutdown()
