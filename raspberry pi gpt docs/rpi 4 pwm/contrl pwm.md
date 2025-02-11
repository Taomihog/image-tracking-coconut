To enable and control both **PWM0 (GPIO12)** and **PWM1 (GPIO13)** on a Raspberry Pi through file writes to `/sys/class/pwm/pwmchip0`, follow these steps:

---

### **1. Enable the PWM Hardware in Device Tree**
Edit the boot configuration to enable hardware PWM.

Open the boot configuration file:
```bash
sudo nano /boot/config.txt
```

Add or uncomment the following lines:
```text
dtoverlay=pwm-2chan
```

Save and reboot the system:
```bash
sudo reboot
```

---

### **2. Export PWM Channels**
After reboot, navigate to the PWM control directory:
```bash
cd /sys/class/pwm/pwmchip0
```

Export both channels (`pwm0` and `pwm1`):
```bash
echo 0 > export
echo 1 > export
```

---

### **3. Set Period and Duty Cycle for Both Channels**
PWM period and duty cycle are set in nanoseconds (`ns`).

- Set the period for each channel:
  ```bash
  echo 1000000 > pwm0/period
  echo 1000000 > pwm1/period
  ```
  This sets a period of 1 ms (1 MHz frequency).

- Set the duty cycle (pulse width) for each channel:
  ```bash
  echo 500000 > pwm0/duty_cycle
  echo 750000 > pwm1/duty_cycle
  ```

---

### **4. Enable PWM Output**
Activate the channels:
```bash
echo 1 > pwm0/enable
echo 1 > pwm1/enable
```

---

### **5. Example Bash Script**
Here's a simple script to control both channels:
```bash
#!/bin/bash

PWM_DIR="/sys/class/pwm/pwmchip0"

# Export both channels
echo 0 > "$PWM_DIR/export"
echo 1 > "$PWM_DIR/export"

# Set period and duty cycle
echo 1000000 > "$PWM_DIR/pwm0/period"
echo 1000000 > "$PWM_DIR/pwm1/period"
echo 500000 > "$PWM_DIR/pwm0/duty_cycle"
echo 750000 > "$PWM_DIR/pwm1/duty_cycle"

# Enable channels
echo 1 > "$PWM_DIR/pwm0/enable"
echo 1 > "$PWM_DIR/pwm1/enable"

echo "PWM signals enabled on both channels."
```

---

### **6. Notes**
- You must run these commands as `sudo`.
- The Raspberry Pi's GPIO pins for PWM are:
  - PWM0: GPIO12 (Physical pin 32)
  - PWM1: GPIO13 (Physical pin 33)
- Ensure no other peripheral is configured on these pins.
- Use proper permissions (`chmod`) or execute as root.

Let me know if you need help troubleshooting or modifying this further!