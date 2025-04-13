# To set up a network between your **Windows machine** and a **Raspberry Pi 4** (both running ROS 2 in Docker), you'll need to configure Docker networking such that both containers can communicate with each other. The key goal is to allow both machines to discover and communicate via ROS 2 topics/services using **DDS (Data Distribution Service)**

Here’s a step-by-step guide to achieve this:

## **1. Docker Networking Mode**

By default, Docker containers use the `bridge` network mode. However, this can restrict communication between containers running on different hosts. To enable communication between your Windows machine and Raspberry Pi 4 (using Docker), you need to configure a **user-defined bridge network**.

## **2. Set Up a User-Defined Network in Docker**

A user-defined bridge network allows containers to communicate directly with each other by their container names.

### On **Windows**

1. Open a terminal (PowerShell or Command Prompt) and create a **user-defined bridge network**:

   ```sh
   docker network create --driver bridge ros_network
   ```

### On **Raspberry Pi**

1. SSH into your Raspberry Pi or access the terminal.

2. Similarly, create the same network on your Raspberry Pi:

   ```sh
   docker network create --driver bridge ros_network
   ```

## **3. Start ROS 2 Containers on Both Machines**

Now, you can run your **ROS 2 containers** on both Windows and Raspberry Pi, and attach them to the same network.

### On **Windows**

Run the ROS 2 container with the following command, replacing `<image_name>` with your ROS 2 Docker image:

```sh
docker run -it --rm --network ros_network --name ros_win_container <image_name>
```

### On **Raspberry Pi**

Run the ROS 2 container similarly:

```sh
docker run -it --rm --network ros_network --name ros_rpi_container <image_name>
```

## **4. Setting Up ROS 2 Communication (DDS Configuration)**

Since ROS 2 uses **DDS** for communication, and DDS typically uses multicast for discovery, we need to make sure that both containers can communicate over the network.

### For **ROS 2 Discovery** to work between Windows and Raspberry Pi, do the following

1. **Set the ROS_DOMAIN_ID** (if needed):
   ROS 2 uses the `ROS_DOMAIN_ID` environment variable to isolate different DDS domains. If you want both machines to be in the same domain, make sure that the `ROS_DOMAIN_ID` is the same on both machines. Set this before running your containers.

   Example for **Windows** (in the terminal or in the Dockerfile):

   ```sh
   set ROS_DOMAIN_ID=0
   ```

   Example for **Raspberry Pi** (in the terminal or in the Dockerfile):

   ```sh
   export ROS_DOMAIN_ID=0
   ```

2. **Ensure Multicast is Enabled** (for discovery):
   ROS 2 uses **multicast** for discovery, so ensure your firewall or network settings allow it. You might need to configure **multicast** on both Windows and Raspberry Pi.

   On **Windows**, you might need to enable multicast in Docker by adding the following lines to your Docker settings (in Docker Desktop settings for Windows):
   - Go to **Settings** -> **Network** -> **Advanced Settings**.
   - Enable **multicast** and **UDP** if needed.

   On the **Raspberry Pi**, multicast should be enabled by default, but you can check by running:

   ```sh
   sudo sysctl net.ipv4.conf.all.mc_forwarding
   ```

   If this command returns `0`, you need to enable multicast forwarding:

   ```sh
   sudo sysctl -w net.ipv4.conf.all.mc_forwarding=1
   ```

## **5. Testing Communication**

Once your containers are running and configured to use the same ROS_DOMAIN_ID, test the communication between the two machines.

1. **On Raspberry Pi**, you can run a ROS 2 publisher (e.g., `talker`):

   ```sh
   ros2 run demo_nodes_cpp talker
   ```

2. **On Windows**, run the corresponding subscriber (e.g., `listener`):

   ```sh
   ros2 run demo_nodes_cpp listener
   ```

If everything is configured correctly, the `listener` on Windows should display the messages published by the `talker` on the Raspberry Pi.

## **6. Troubleshooting Tips**

- **Check Docker Logs**: If communication doesn’t work, check the logs of your Docker containers to make sure they are running correctly:

  ```sh
  docker logs ros_win_container
  docker logs ros_rpi_container
  ```
  
- **Ping the Containers**: Ensure that both containers can ping each other (from one container to another) over the network. If you use the `--network` flag when running the containers, they should be able to communicate by their container names (like `ros_win_container` and `ros_rpi_container`).
  
- **Check Firewall Settings**: Make sure that the firewall on both Windows and Raspberry Pi isn’t blocking multicast traffic or ROS 2 communication ports.

---

## **Summary**

1. Create a **user-defined Docker network** on both Windows and Raspberry Pi.
2. Run your **ROS 2 containers** using that network.
3. Set **ROS_DOMAIN_ID** to the same value on both systems.
4. Ensure **multicast** is enabled for DDS discovery.
5. Run ROS 2 nodes and test communication between the systems.

Let me know if you encounter any issues or need further clarification!
