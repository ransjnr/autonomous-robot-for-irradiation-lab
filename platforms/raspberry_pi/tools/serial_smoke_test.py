#!/usr/bin/env python3
"""Non-motion serial health check; safe to run with motor power disconnected."""
import argparse
import sys
import time
import serial

def main():
    parser=argparse.ArgumentParser(); parser.add_argument('--port',required=True); parser.add_argument('--baud',type=int,default=115200); args=parser.parse_args()
    with serial.Serial(args.port,args.baud,timeout=1) as link:
        time.sleep(2)  # Uno resets when USB serial opens
        link.reset_input_buffer(); link.write(b'PING\n'); reply=link.readline().decode(errors='replace').strip()
    if reply != 'PONG':
        print(f'Expected PONG, received {reply!r}',file=sys.stderr); return 1
    print('PASS: controller replied PONG'); return 0
if __name__ == '__main__': raise SystemExit(main())
