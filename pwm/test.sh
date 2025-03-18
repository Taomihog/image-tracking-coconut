#!/bin/bash
g++ servo_ctrl.cpp test.cpp -o test
sudo ./test
rm test
