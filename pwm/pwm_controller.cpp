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
	period = period_ns;
	freq = 1000000000.0 / period;
        std::cout << "PWM period set to " << period_ns << " ns." << std::endl;
    }

    void setFreq(double freq_Hz) {
	unsigned int period_ns = (unsigned int) (1000000000.0/freq_Hz);
	setPeriod(period_ns);
	period = period_ns;
	freq = freq_Hz;
	std::cout << "PWM frequency set to " << freq_Hz << " Hz." << std::endl;
    }

    void setDutyCycle(unsigned int duty_ns) {
        writeToFile(PWM_DUTY_CYCLE, std::to_string(duty_ns));
	duty = duty_ns;
	duty_cycle_ratio = (double) duty/period;
        std::cout << "PWM duty cycle set to " << duty_ns << " ns." << std::endl;
    }

    void setDutyRatio(double ratio) {
	duty_cycle_ratio = ratio;
	duty = (unsigned int) (period * ratio);
	setDutyCycle(duty);
	std::cout << "PWM duty cycle ratio set to " << ratio << "." << std::endl;
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
    unsigned int period; // ns
    unsigned int duty; //ns
    double freq; // Hz
    double duty_cycle_ratio; // 0 - 1
};

int main() {
    PWMController pwm;

    // Gradually modulate duty cycle
    double dfreq = 100;
    double steps = 200; 
    pwm.setFreq(0);

    while (true) {
        for (unsigned int i = 0; i <= steps; ++i) {
            pwm.setDutyCycle(dfreq * i);
            usleep(100000); // Wait 100ms
        }
    }

    return 0;
}

