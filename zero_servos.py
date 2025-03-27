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

send(0, 48)
time.sleep(0.1)
send(1, 47)
time.sleep(0.1)
