# You can run **ROS 2 in a Docker container** and write ROS 2 nodes inside it. I'll guide you step by step on setting up a **ROS 2 Docker environment** and writing **publisher/subscriber nodes** in Python and C++

---

## **1. Set Up a ROS 2 Docker Container**

### **Pull the ROS 2 Image**

Run the following to download a ROS 2 **Humble** container:

```sh
docker pull osrf/ros:humble
```

Other available versions: `foxy`, `iron`, etc.

### **Run the ROS 2 Container**

Start a container with an interactive shell:

```sh
docker run -it --rm --name ros2_container osrf/ros:humble bash
```

This launches a **temporary container** that removes itself after exit (`--rm`).

If you want to keep the container running in the background:

```sh
docker run -it --name ros2_container -v $(pwd):/ros2_ws osrf/ros:humble bash
```

- The `-v $(pwd):/ros2_ws` option mounts your current directory to `/ros2_ws` inside the container.
- You can edit files from your host machine.

---

## **2. Create a ROS 2 Workspace in the Container**

Inside the running container, set up a **ROS 2 workspace**:

```sh
mkdir -p /ros2_ws/src
cd /ros2_ws
rosdep update
colcon build
source install/setup.bash
```

---

## **3. Write a ROS 2 Node**

### **Python Node (Publisher)**

Inside `/ros2_ws/src`, create a package:

```sh
cd /ros2_ws/src
ros2 pkg create --build-type ament_python my_package
```

Edit the node file (`my_package/my_package/talker.py`):

```python
import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class Talker(Node):
    def __init__(self):
        super().__init__("talker")
        self.publisher_ = self.create_publisher(String, "chatter", 10)
        self.timer = self.create_timer(1.0, self.publish_message)

    def publish_message(self):
        msg = String()
        msg.data = "Hello from Docker ROS 2!"
        self.publisher_.publish(msg)
        self.get_logger().info(f"Publishing: {msg.data}")

def main(args=None):
    rclpy.init(args=args)
    node = Talker()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
```

Make it executable:

```sh
chmod +x my_package/talker.py
```

Add this to `setup.py`:

```python
entry_points={
    'console_scripts': [
        'talker = my_package.talker:main',
    ],
}
```

### **C++ Node (Subscriber)**

Inside `/ros2_ws/src`, create a C++ package:

```sh
cd /ros2_ws/src
ros2 pkg create --build-type ament_cmake my_cpp_package
```

Edit `src/listener.cpp`:

```cpp
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Listener : public rclcpp::Node {
public:
    Listener() : Node("listener") {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "chatter", 10, std::bind(&Listener::callback, this, std::placeholders::_1));
    }

private:
    void callback(const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Listener>());
    rclcpp::shutdown();
    return 0;
}
```

Add to `CMakeLists.txt`:

```cmake
add_executable(listener src/listener.cpp)
ament_target_dependencies(listener rclcpp std_msgs)
install(TARGETS listener DESTINATION lib/${PROJECT_NAME})
```

---

## **4. Build and Run the Nodes**

### **Build Everything**

```sh
cd /ros2_ws
colcon build
source install/setup.bash
```

### **Run the Publisher**

```sh
ros2 run my_package talker
```

### **Run the Subscriber**

In a new terminal, enter the container:

```sh
docker exec -it ros2_container bash
```

Run the subscriber:

```sh
ros2 run my_cpp_package listener
```

Now, you should see the messages being published and received inside the **Docker container**!

---

### **Final Notes**

- Use `docker commit` if you want to **save the container state**.
- You can add a `Dockerfile` to automate the workspace setup.

Would you like a `Dockerfile` for a ROS 2 development environment? 🚀

Sure! Below is a **Dockerfile** to set up a **ROS 2 Humble development environment** with Python and C++ support. It also includes workspace initialization.

---

### **Dockerfile for ROS 2 Development**

```dockerfile
# Use official ROS 2 Humble image as base
FROM osrf/ros:humble

# Set environment variables
ENV ROS_WS=/ros2_ws

# Install development tools and dependencies
RUN apt-get update && apt-get install -y \
    python3-pip \
    python3-colcon-common-extensions \
    ros-humble-rclcpp \
    ros-humble-std-msgs \
    && rm -rf /var/lib/apt/lists/*

# Create a ROS 2 workspace
RUN mkdir -p $ROS_WS/src && \
    cd $ROS_WS && colcon build

# Set working directory
WORKDIR $ROS_WS

# Source ROS 2 setup when starting a shell
RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc && \
    echo "source $ROS_WS/install/setup.bash" >> ~/.bashrc

# Default command: Start a Bash shell
CMD ["bash"]
```

---

### **How to Build and Use the Docker Image**

#### **1. Build the Image**

Run this in the directory containing the **Dockerfile**:

```sh
docker build -t ros2-dev .
```

#### **2. Run the Container**

Start the container and mount your local workspace:

```sh
docker run -it --name ros2_container -v $(pwd):/ros2_ws ros2-dev
```

- `-v $(pwd):/ros2_ws` allows you to **edit files from your host** and use them inside Docker.

#### **3. Create a ROS 2 Package Inside Docker**

```sh
cd /ros2_ws/src
ros2 pkg create --build-type ament_python my_python_pkg
ros2 pkg create --build-type ament_cmake my_cpp_pkg
cd /ros2_ws && colcon build
```

#### **4. Run Nodes**

```sh
source install/setup.bash
ros2 run my_python_pkg <node_name>
ros2 run my_cpp_pkg <node_name>
```

---

### **Customization**

- You can add `vim`, `nano`, or `git` for easier development:

  ```dockerfile
  RUN apt-get install -y nano git
  ```

Would you like help with a **Docker Compose file** for managing multiple ROS 2 containers? 🚀
