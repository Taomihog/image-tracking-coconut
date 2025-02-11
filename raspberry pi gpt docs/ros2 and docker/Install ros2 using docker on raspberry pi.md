## Install ros2 using docker on raspberry pi

### **Installing ROS 2 on Raspberry Pi using Docker**

Here’s a step-by-step guide to set up ROS 2 (Humble or later) on your Raspberry Pi using Docker.

---

### **1. Install Docker on Raspberry Pi**
Ensure Docker is installed and set up properly:

```bash
sudo apt-get update
sudo apt-get install -y docker.io
```

(Optional) Enable Docker to start on boot:

```bash
sudo systemctl enable docker
sudo systemctl start docker
```

Verify Docker installation:

```bash
docker --version
```

---

### **2. Pull the ROS 2 Docker Image**
Choose your desired ROS 2 version and pull the official image from Docker Hub:

```bash
docker pull osrf/ros:humble-desktop
```

Alternatively, for `foxy`, `galactic`, or `rolling`:

```bash
docker pull osrf/ros:foxy
```

---

### **3. Run the ROS 2 Container**
Start a container and open an interactive shell:

```bash
docker run -it --rm osrf/ros:humble-desktop bash
```

If you need GUI support (for tools like RViz):

```bash
xhost +
docker run -it --rm \
  --net=host \
  --env="DISPLAY=$DISPLAY" \
  --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
  osrf/ros:humble-desktop bash
```

---

### **4. Test ROS 2 Installation**
Inside the container:

```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_cpp talker
```

Open a new terminal to run the listener node:

```bash
docker exec -it <container_name> bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_cpp listener
```

---

### **5. (Optional) Save Container State**
To create a reusable image after customizing:

```bash
docker commit <container_id> ros2-custom
docker run -it ros2-custom bash
```

---

### **6. Docker Compose (Optional)**
Create a `docker-compose.yml` for persistent setups:

```yaml
version: '3'
services:
  ros2:
    image: osrf/ros:humble-desktop
    container_name: ros2_container
    network_mode: "host"
    environment:
      - DISPLAY=${DISPLAY}
    volumes:
      - /tmp/.X11-unix:/tmp/.X11-unix
```

Run it:

```bash
docker-compose up -d
```

---

Let me know if you'd like additional details or examples for custom ROS packages.