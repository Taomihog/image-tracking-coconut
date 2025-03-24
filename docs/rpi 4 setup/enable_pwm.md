# Enable PWM

```bash
sudo vim /boot/firmware/config.txt
```

add this

```bash
dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
```

check

```bash
pinctrl 12
pinctrl 13
```

set if pin 12 and 13 don't work

```bash
pinctrl 12 a0
pinctrl 13 a0
```

example servo control program servo_controller.cpp

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
