"""Local-network phone controller. Bind only to a trusted robot Wi-Fi network."""
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from threading import Thread
import json, rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist

PAGE = '''<!doctype html><meta name=viewport content="width=device-width,initial-scale=1"><style>body{font:24px sans-serif;text-align:center;background:#111;color:#eee}button{width:30%;height:80px;margin:8px;font-size:28px}#stop{background:#d33;color:white}</style><h2>Robot control</h2><p><button data-v=1 data-w=0>▲</button></p><button data-v=0 data-w=1>◀</button><button id=stop>STOP</button><button data-v=0 data-w=-1>▶</button><p><button data-v=-1 data-w=0>▼</button></p><script>let t;function go(v,w){fetch('/drive',{method:'POST',body:JSON.stringify({v,w})})}function down(e){let b=e.currentTarget,v=b.dataset.v,w=b.dataset.w;t=setInterval(()=>go(v,w),100);go(v,w)}function up(){clearInterval(t);go(0,0)}document.querySelectorAll('[data-v]').forEach(b=>{b.onpointerdown=down;b.onpointerup=up;b.onpointerleave=up;b.onpointercancel=up});document.querySelector('#stop').onclick=up;</script>'''
class Control(Node):
    def __init__(self):
        super().__init__('phone_controller'); self.pub=self.create_publisher(Twist,'/cmd_vel/request',10)
        self.create_timer(.35,self.stop) # dead-man timeout, buttons must be repeatedly pressed/refreshed
    def move(self,v,w):
        m=Twist(); m.linear.x=.35*float(v); m.angular.z=1.2*float(w); self.pub.publish(m)
    def stop(self): self.move(0,0)
def main():
    rclpy.init(); node=Control()
    class Handler(BaseHTTPRequestHandler):
        def do_GET(self): self.send_response(200); self.send_header('Content-Type','text/html'); self.end_headers(); self.wfile.write(PAGE.encode())
        def do_POST(self):
            try:
                data=json.loads(self.rfile.read(int(self.headers['Content-Length']))); node.move(data['v'],data['w']); self.send_response(204); self.end_headers()
            except Exception: self.send_error(400)
        def log_message(self,*args): pass
    server=ThreadingHTTPServer(('0.0.0.0',8080),Handler); Thread(target=server.serve_forever,daemon=True).start()
    node.get_logger().info('Phone controller listening on port 8080')
    try: rclpy.spin(node)
    except KeyboardInterrupt: pass
    finally: server.shutdown(); node.destroy_node(); rclpy.shutdown()
