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
static void writeToFile(const std::string &path, const std::string &val) {
    std::ofstream file(path);
    if(file.is_open()) {
	file << val;
	file.close();
    } else {
	std::cerr << "Error: cannot write to file!" << std::endl;
    }
}

int main() {
    if (std::ifstream file(PWM_CHIP); !file.good()) {
	return 0;
    }

    writeToFile(PWM_EXPORT, "0");
    usleep(100000);

    writeToFile(PWM_PERIOD, "1000000");
    writeToFile(PWM_DUTY_CYCLE, "500000");
    writeToFile(PWM_ENABLE, "1");
    double max = 5000; // max Hz
    double step = 100;// 100 Hz step, so total 100 steps
    double curr = 0;
    while(curr < max) {
	curr += 10;
	int period = static_cast<int>(1000000000.0 / curr);
        writeToFile(PWM_PERIOD, std::to_string(period)); 
	writeToFile(PWM_DUTY_CYCLE, std::to_string(static_cast<int>(period/2.0)));
	usleep(50000); // 50 ms
        // std::cout << "current frequency: " << curr << " Hz" << std::endl;
    }
    writeToFile(PWM_ENABLE, "0");
    return 0;
}
