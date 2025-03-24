#!/bin/bash
g++ servo_ctrl.cpp test.cpp -o test_rpi4
sudo ./test_rpi4
rm test_rpi4
