#!/usr/bin/env python3
import serial
import time
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.1)
    ser.flush()
    while True:
        a = 127
        ser.write(int(a))
        #line = ser.readline().decode('utf-8').rstrip()
        #print(line)
        time.sleep(1)