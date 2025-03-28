#!/bin/bash
g++ -std=c++20 main.cpp servo_328p.cpp servo_rpi.cpp -o test_servo
sudo ./test_servo
rm test_servo
