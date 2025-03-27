#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install build-essential cmake arduino libopencv-dev python3-opencv fswebcam vim -y
sudo usermod -aG dialout $USER

sudo bash -c 'echo "dtoverlay=pwm-2chan,func=gpio,pin=12,pin2=13" >> /boot/firmware/config.txt'

sudo pinctrl 12 a0
sudo pinctrl 13 a0

sudo reboot
