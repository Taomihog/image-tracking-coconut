## Connect to Raspberry Pi

ssh taomihog@10.0.0.184

docker pull ros:humble-ros-core
docker run -it --rm ros:humble-ros-core

taomihog@raspberrypi:~ $ docker run -it --name ros_container ros:humble-ros-core