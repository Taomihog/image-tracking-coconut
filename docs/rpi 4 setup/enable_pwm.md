# To enable and control both **PWM0 (GPIO12)** and **PWM1 (GPIO13)** on a Raspberry Pi through file writes to `/sys/class/pwm/pwmchip0`, follow these steps

## **1. Enable the PWM Hardware in Device Tree**

To enable both PWMs (GPIO pin 12 and 13):

```bash
sudo vim /boot/firmware/config.txt
```

add this

```bash
dtoverlay=pwm-2chan,func=gpio,pin=12,pin2=13
```

check if both are activate using:

```bash
pinctrl 12
pinctrl 13
```

if they are not pwm0_0 and pwm0_1, do this command:

```bash
pinctrl 12 a0
pinctrl 13 a0
```

---

## **3. Set Period and Duty Cycle for Both Channels**

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

## **4. Enable PWM Output**

Activate the channels:

```bash
echo 1 > pwm0/enable
echo 1 > pwm1/enable
```

---

## **5. Example Bash Script**

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

## **6. Notes**

- You must run these commands as `sudo`.
- The Raspberry Pi's GPIO pins for PWM are:
  - PWM0: GPIO12 (Physical pin 32)
  - PWM1: GPIO13 (Physical pin 33)
- Ensure no other peripheral is configured on these pins.
- Use proper permissions (`chmod`) or execute as root.

Let me know if you need help troubleshooting or modifying this further!

## **7. example servo control program**

servo_controller.cpp

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> // for usleep

#define PWM_CHIP "/sys/class/pwm/pwmchip0/"
#define PWM_EXPORT PWM_CHIP "export"
#define PWM_PWM0 PWM_CHIP "pwm0/"
#define PWM_PERIOD PWM_PWM0 "period"
#define PWM_DUTY_CYCLE PWM_PWM0 "duty_cycle"
#define PWM_ENABLE PWM_PWM0 "enable"

class PWMController {
public:
    PWMController() {
        // Export PWM0
        if (!fileExists(PWM_PWM0)) {
            writeToFile(PWM_EXPORT, "0");
            usleep(100000); // Allow time for PWM0 to be created
        }

        setPeriod(1000000); // Default period of 1ms (1 kHz frequency)
        setDutyCycle(500000); // 50% duty cycle
        enable(true);
    }

    ~PWMController() {
        enable(false);
        std::cout << "PWM disabled." << std::endl;
    }

    void setPeriod(unsigned int period_ns) {
        writeToFile(PWM_PERIOD, std::to_string(period_ns));
        std::cout << "PWM period set to " << period_ns << " ns." << std::endl;
    }

    void setDutyCycle(unsigned int duty_ns) {
        writeToFile(PWM_DUTY_CYCLE, std::to_string(duty_ns));
        std::cout << "PWM duty cycle set to " << duty_ns << " ns." << std::endl;
    }

    void enable(bool enable) {
        writeToFile(PWM_ENABLE, enable ? "1" : "0");
        std::cout << "PWM " << (enable ? "enabled" : "disabled") << "." << std::endl;
    }

private:
    void writeToFile(const std::string &path, const std::string &value) {
        std::ofstream file(path);
        if (file.is_open()) {
            file << value;
            file.close();
        } else {
            std::cerr << "Error: Unable to write to " << path << std::endl;
        }
    }

    bool fileExists(const std::string &path) {
        std::ifstream file(path);
        return file.good();
    }
};

int main() {
    PWMController pwm;

    // Gradually modulate duty cycle
    unsigned int max_duty = 1000000; // 100% duty cycle
    unsigned int step = 100000; // Step size for duty cycle modulation

    while (true) {
        for (unsigned int duty = 0; duty <= max_duty; duty += step) {
            pwm.setDutyCycle(duty);
            usleep(500000); // Wait 500ms
        }

        for (unsigned int duty = max_duty; duty > 0; duty -= step) {
            pwm.setDutyCycle(duty);
            usleep(500000); // Wait 500ms
        }
    }

    return 0;
}
```
