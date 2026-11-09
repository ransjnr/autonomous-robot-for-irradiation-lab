"""Converts monotonically reported wheel ticks into odom and odom->base_link TF."""
import json, math
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from nav_msgs.msg import Odometry
from geometry_msgs.msg import TransformStamped
from tf2_ros import TransformBroadcaster

class WheelOdometry(Node):
    def __init__(self):
        super().__init__('wheel_odometry')
        for name, value in [('wheel_radius_m',.0325),('wheel_base_m',.145),('ticks_per_revolution',20.0)]: self.declare_parameter(name,value)
        self.pub=self.create_publisher(Odometry,'/odom',20); self.tf=TransformBroadcaster(self)
        self.last=None; self.x=self.y=self.yaw=0.; self.create_subscription(String,'/robot/telemetry',self.update,20)
    def update(self,msg):
        try: d=json.loads(msg.data); ticks=(int(d['left_ticks']),int(d['right_ticks']))
        except (ValueError, KeyError, json.JSONDecodeError): return
        now=self.get_clock().now()
        if self.last is None: self.last=(ticks,now); return
        old, stamp=self.last; dt=(now-stamp).nanoseconds/1e9; self.last=(ticks,now)
        if dt<=0 or dt>1: return
        circumference=2*math.pi*self.get_parameter('wheel_radius_m').value
        meters_per_tick=circumference/self.get_parameter('ticks_per_revolution').value
        dl=(ticks[0]-old[0])*meters_per_tick; dr=(ticks[1]-old[1])*meters_per_tick
        ds=(dl+dr)/2; dyaw=(dr-dl)/self.get_parameter('wheel_base_m').value
        self.x+=ds*math.cos(self.yaw+dyaw/2); self.y+=ds*math.sin(self.yaw+dyaw/2); self.yaw+=dyaw
        qz=math.sin(self.yaw/2); qw=math.cos(self.yaw/2)
        odom=Odometry(); odom.header.stamp=now.to_msg(); odom.header.frame_id='odom'; odom.child_frame_id='base_link'; odom.pose.pose.position.x=self.x; odom.pose.pose.position.y=self.y; odom.pose.pose.orientation.z=qz; odom.pose.pose.orientation.w=qw; odom.twist.twist.linear.x=ds/dt; odom.twist.twist.angular.z=dyaw/dt; self.pub.publish(odom)
        tf=TransformStamped(); tf.header=odom.header; tf.child_frame_id='base_link'; tf.transform.translation.x=self.x; tf.transform.translation.y=self.y; tf.transform.rotation.z=qz; tf.transform.rotation.w=qw; self.tf.sendTransform(tf)
def main():
    rclpy.init(); n=WheelOdometry()
    try:rclpy.spin(n)
    except KeyboardInterrupt:pass
    finally:n.destroy_node();rclpy.shutdown()
