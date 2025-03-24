# install wsl and ros on windows

install wsl

```powershell
wsl --install
wsl --set-default-version 2
```

check version and info

```powershell
wsl --status
wsl -v
```

set env? check help file

```powershell
$env:WSL_UTF8=1
wsl --help | more
```

output help file to home

```powershell
wsl --help > wsl.help
explorer.exe .
notepade wsl.help
```

get the available distros

```powershell
wsl --list --online
```

install
for the next command, when prompted
use: username for the username
use: password for the password

```powershell
wsl.exe --install --d Ubuntu-22.04
```

get the home for the Ubuntu-22.04

```powershell
explorer.exe .
```

exit

```powershell
exit
```

list all installed

```powershell
wsl -l
```

set default to Ubuntu-22.04

```powershell
wsl --set-default Ubuntu-22.04

run
```powershell
wsl
```

(in case you need to uninstall it) unregister the distribution and delete the root filesystem.

```powershell
wsl.exe --unregister Ubuntu-22.04
```

update necessary packages

```bash
sudo apt install locales curl gnupg lsb-release -y
```

set locale to UTF-8

```bash
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8
```

enable Ubuntu Universe repository

```bash
sudo apt install software-properties-common
sudo add-apt-repository universe
```

add the ROS 2 GPG key

```bash
sudo apt update && sudo apt install curl -y
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
```

add the repository to your sources list

```bash
echo "deb [signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
```

add the repository to your sources list

```bash
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
```

desktop Install (Recommended): ROS, RViz, demos, tutorials

```bash
sudo apt install ros-humble-desktop
```

ROS-Base Install (Bare Bones): Communication libraries, message packages, command line tools. No GUI tools.

```bash
sudo apt install ros-humble-ros-base
```

development tools: compilers and other tools to build ROS packages

```bash
sudo apt install ros-dev-tools
```

source the ROS 2 setup script

```bash
source /opt/ros/humble/setup.bash
```

to automatically source this script in every new terminal session, add the above line to your ~/.bashrc file

```bash
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
```

test the installation by running

```bash
ros2 run demo_nodes_cpp talker
```
