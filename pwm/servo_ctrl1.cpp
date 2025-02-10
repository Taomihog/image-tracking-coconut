#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> // for usleep
#include "servo_ctrl.hpp"

servo::servo(int dev_in): dev(dev_in) {
    if(dev != 0 && dev != 1) {
        std::cout << "dev doesn't exist, use dev = 0." << std::endl;
        dev = 0;
    }

    std::string path_export = PATH + "/export";
    if (writeToFile(path_export, std::to_string(dev))) {
	path_period =     PATH + "/pwm" + std::to_string(dev) + "/period";
	path_duty_cycle = PATH + "/pwm" + std::to_string(dev) + "/duty_cycle";
	path_enable =     PATH + "/pwm" + std::to_string(dev) + "/enable";
}

servo::~servo() {
    std::string path_unexport = PATH + "/unexport";
    writeToFile(path_unexport, std::to_string(dev));
}

bool servo::enable() {
    if (writeToFile(path_enable, "1")) {
	writeToFile(path_period, std::to_string(PERIOD));
	return true;
    } else {
	return false;
    }
}

bool servo::disable() {
    return writeToFile(path_enable, "0");
}

double servo::rotate(double fraction) {
    if (fraction > 1) {
	fraction = 1;
    }
    if (fraction < 0) {
	fraction = 0;
    }

    const int target_duty_cycle = static_cast<int>(fraction * (DC_H - DC_L)) + DC_L;
    writeToPath(path_duty_cycle, std::to_string(target_duty_cycle));
    return fraction;
}

bool servo::writeToFile(const std::string &path, const std::string &val) {
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
    servo s0(0);
    servo s1(1);
    s0.enable();
    s1.enable();
    double f = 0;
    while(f < 1) {
	s0.rotate_to(f);
	s1.rotate_to(f);
	f += 0.05;
	usleep(100000);
    }
    s0.disable();
    s1.disable();
    return 0;
}
