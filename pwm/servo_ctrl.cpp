#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> // for usleep
#include "servo_ctrl.hpp"

Servo::Servo(int dev_in): dev(dev_in) {
    if(dev != 0 && dev != 1) {
        std::cout << "dev doesn't exist, use dev = 0." << std::endl;
        dev = 0;
    }

    std::string path_export = PATH + "/export";
    if (WriteToFile(path_export, std::to_string(dev))) {
	path_period =     PATH + "/pwm" + std::to_string(dev) + "/period";
	path_duty_cycle = PATH + "/pwm" + std::to_string(dev) + "/duty_cycle";
	path_enable =     PATH + "/pwm" + std::to_string(dev) + "/enable";
    }
}

Servo::~Servo() {
    std::string path_unexport = PATH + "/unexport";
    WriteToFile(path_unexport, std::to_string(dev));
}

bool Servo::Enable() {
    if (WriteToFile(path_enable, "1")) {
	WriteToFile(path_period, std::to_string(PERIOD));
	return true;
    } else {
	return false;
    }
}

bool Servo::Disable() {
    return WriteToFile(path_enable, "0");
}

double Servo::Rotate_to(double fraction) {
    if (fraction > 1) {
	fraction = 1;
    }
    if (fraction < 0) {
	fraction = 0;
    }

    const int target_duty_cycle = static_cast<int>(fraction * (DC_H - DC_L)) + DC_L;
    WriteToFile(path_duty_cycle, std::to_string(target_duty_cycle));
    return fraction;
}

bool Servo::WriteToFile(const std::string &path, const std::string &val) {
    std::ofstream file(path);
    if(file.is_open()) {
	file << val;
	file.close();
	return true;
    } else {
	std::cerr << "Error: cannot write to file!" << std::endl;
	return false;
    }
}

int main() {
    Servo *s[] = {new Servo(0), new Servo(1)};
    s[0]->Enable();
    s[1]->Enable();
    double f = 0.0;
    constexpr double start[] = {0.43,0.0};
    constexpr double end[] = {0.92, 1.0};
    constexpr int steps = 1000;
    constexpr double incre[] = {(end[0] - start[0]) / steps, (end[1] - start[1]) / steps};
    constexpr double total_time = 5.0; //second
    constexpr double step_time = total_time * 1000.0 * 1000.0 / steps;
    for(int i = 0; i < 1000; ++i) {
	s[0]->Rotate_to(start[0] + incre[0] * i);
	s[1]->Rotate_to(start[1] + incre[1] * i);
	usleep(step_time);
    }
    s[0]->Rotate_to(start[0] + 0.05);
    s[1]->Rotate_to((start[1] + end[1]) / 2.0 + 0.05);
    usleep(1000*1000);
    s[0]->Rotate_to(start[0]);
    s[1]->Rotate_to((start[1] + end[1]) / 2.0);
    usleep(200*1000);
    s[0]->Disable();
    s[1]->Disable();
    return 0;
}
