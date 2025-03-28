// program Atmega328P and establish serial communication with a servo controller using the Arduino IDE
#pragma once
#include <mutex>
#include <iostream>
#include <fcntl.h>      // File control definitions
#include <termios.h>    // POSIX terminal control definitions
#include <unistd.h>     // UNIX standard function definitions
#include <cstring>      // For memset


// python code
// import time
// import serial
// import struct

// # Open serial connection
// ino = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
// time.sleep(2)  # Wait for Arduino to initialize

// def send(pwm_id, value):
//     if pwm_id < 0 or pwm_id > 1:
//         print("Invalid PWM ID")
//         return
    
//     message = (pwm_id << 28) |  (value & 0xFFFF)
//     ino.write(struct.pack('<I', message))

// send(0, 48) // set continous servo 0 to speed to 0.48 (max = 100%)
// time.sleep(0.1)
// send(1, 44) // set servo 1 to 44%
// time.sleep(0.1)

class Servo_328P : public Servo_base {
    // This class controls a servo motor on an Atmega328P microcontroller using the serial communication interface
public:
    Servo_328P(int dev_in);
    ~Servo_328P();
    double Rotate_to(double fraction) override;
private:
    static std::mutex mtx; // Mutex for thread safety when sending commands to the microcontroller
    static int serial_fd = -1; // Serial file descriptor for communication with the microcontroller
    static int n_dev = 0;
    int open_serial();
};
