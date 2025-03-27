import time
import serial
import struct

# Open serial connection
ino = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
time.sleep(2)  # Wait for Arduino to initialize

def send(pwm_id, value):
    if pwm_id < 0 or pwm_id > 1:
        print("Invalid PWM ID")
        return
    
    message = (pwm_id << 28) |  (value & 0xFFFF)
    ino.write(struct.pack('<I', message))

dir = True
i = 0
steps = 100
try:
    while(1):
        if i == 1000:
            break
        if dir:
            i = i + 1
        else:
            i = i - 1
        if i == 0 or i == steps:
            dir = not dir
        send(0, i) 
        time.sleep(0.1)
        send(1, i)
        time.sleep(0.1)
except KeyboardInterrupt:
    send(0, 49)
    time.sleep(0.1)
    send(1, 49)
    time.sleep(0.1)
