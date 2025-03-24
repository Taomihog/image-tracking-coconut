#!/bin/bash
cd servo_rpi4
g++ servo_ctrl.cpp servo_test.cpp -o servo_test
cd ..
sudo ./servo_test
rm ./servo_test
