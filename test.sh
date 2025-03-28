#!/bin/bash
g++ -std=c++20 main.cpp Servo_328P.cpp Servo_rpi.cpp -o test_servo
sudo ./test_servo
rm test_servo
python test/rpi_328p_set_servos.py
