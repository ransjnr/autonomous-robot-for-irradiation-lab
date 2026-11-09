"""Camera barcode/QR detector. Requires opencv-contrib-python for 1D barcode support."""
import json, cv2, rclpy
from rclpy.node import Node
from std_msgs.msg import String
class Detector(Node):
    def __init__(self):
        super().__init__('barcode_detector'); self.declare_parameter('camera_source','0')
        source=self.get_parameter('camera_source').value
        self.cap=cv2.VideoCapture(int(source) if source.isdigit() else source); self.pub=self.create_publisher(String,'/barcode_scan',10)
        self.detector = cv2.barcode_BarcodeDetector() if hasattr(cv2,'barcode_BarcodeDetector') else None
        self.qr=cv2.QRCodeDetector(); self.last=set(); self.create_timer(.04,self.tick)
    def tick(self):
        ok, frame=self.cap.read()
        if not ok: return
        values=[]
        if self.detector:
            try:
                decoded, _, _ = self.detector.detectAndDecodeWithType(frame); values += [x for x in decoded if x]
            except cv2.error: pass
        text, points, _=self.qr.detectAndDecode(frame)
        if text: values.append(text)
        for value in set(values):
            if value not in self.last:
                self.pub.publish(String(data=json.dumps({'data':value,'stamp_ns':self.get_clock().now().nanoseconds}))); self.get_logger().info(f'Scan: {value}')
        self.last=set(values)
    def destroy_node(self): self.cap.release(); super().destroy_node()
def main():
    rclpy.init(); n=Detector()
    try: rclpy.spin(n)
    except KeyboardInterrupt: pass
    finally: n.destroy_node(); rclpy.shutdown()
