# Raspberry Pi deployment

1. Create a non-root user named `robot`; clone this repository into `/home/robot/mobile_robot_stack`.
2. Run `platforms/raspberry_pi/setup/provision_pi.sh`, then build `ros2_ws`.
3. Use a stable USB rule or update the serial port in `manual_control.launch.py`; `/dev/ttyACM0` can change after reboot.
4. Copy `systemd/mobile-robot-manual.service` to `/etc/systemd/system/`, then run `sudo systemctl daemon-reload && sudo systemctl enable --now mobile-robot-manual`.

The phone-control server has no authentication. Put the Pi and phone on a dedicated, trusted Wi-Fi network; do not expose port 8080 to the internet.
