
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <array>
#include <thread>
#include <climits>
#include <atomic>
#include <unistd.h> // for usleep
#include "servo_ctrl.hpp"

namespace {
const int number_of_servos = 2;
constexpr double start[] = {0.45,0.0}; // 0.43 is the min for servo 0
constexpr double end[] = {0.85, 1.0}; // 0.92 is the max for servo 0
constexpr int steps = 500;
constexpr double incre[] = {(end[0] - start[0]) / steps, (end[1] - start[1]) / steps};
constexpr double idle[] = {start[0], (start[1] + end[1]) / 2.0};
constexpr double midpoint[] = {(start[0] + end[0]) / 2.0, (start[1] + end[1]) / 2.0};
constexpr double radius[] = {(end[0] - start[0]) / 3.0, (end[1] - start[1]) / 3.0};
constexpr double total_time = 5.0; //second
constexpr double step_time = total_time * 1000.0 * 1000.0 / steps;
constexpr double step_angle = 2 * 3.1415926 / steps;
}

/*
int main() {
    Servo s(0);
    s.Enable();
    for (int i = 0; i < steps; ++i) {
	s.Rotate_to(start[0] + incre[0] * i);
	usleep(step_time);
    }
    s.Disable();
    return 0;
}
*/


void helper_look_around(std::array<Servo, 2>& servos) {
    for (int i = 0; i < steps; ++i) {
        servos[0].Rotate_to(start[0] + incre[0] * i);
        usleep(step_time);
    }
    for (int i = 0; i < steps; ++i) {
        servos[1].Rotate_to(start[1] + incre[1] * i);
        usleep(step_time);
    }
    for (int i = steps - 1; i >= 0; --i) {
        servos[0].Rotate_to(start[0] + incre[0] * i);
        usleep(step_time);
    }
    for (int i = steps - 1; i >= 0; --i) {
        servos[1].Rotate_to(start[1] + incre[1] * i);
        usleep(step_time);
    }
}

void helper_look_around_2(std::array<Servo, 2>& s) {
    for (int i = 0; i < steps; ++i) {
	s[0].Rotate_to(midpoint[0] + 0.5 * (end[0] - start[0]) * sin(step_angle * i));
	s[1].Rotate_to(midpoint[1] + 0.5 * (end[1] - start[1]) * cos(step_angle * i));
	usleep(step_time);
    }
}

std::atomic<bool> running(true);
void spinLoop() {
    std::array<Servo, 2> servos = {Servo(0), Servo(1)};
    
    for (auto& s: servos) {
	s.Enable();
    }
    int i = 0;
    while(running.load() && i < INT_MAX) {
        //helper_look_around(servos);
        //helper_look_around_2(servos);
	servos[0].Rotate_to(0.5 * (start[0] + end[0]) + radius[0] * sin(step_angle * i));
	servos[1].Rotate_to(0.5 * (start[1] + end[1]) + radius[1] * cos(step_angle * i));
	usleep(step_time);
	++i;
    }

    for (int j = 0; j < servos.size(); ++j) {
	servos[j].Rotate_to(idle[j] + 0.05);
    }
    usleep(1000 * 1000);
    for (int j = 0; j < servos.size(); ++j) {
	servos[j].Rotate_to(idle[j]);
    }
    usleep(100 * 1000);  
    for (auto& s: servos) {
	s.Disable();
    }
}

void waitForInput() {
    std::cout << "Press any key to stop the spin loop...\n";
    std::cin.get();
    running.store(false);
}

int main() {
    std::thread spinner(spinLoop);
    std::thread inputListener(waitForInput);

    spinner.join();
    inputListener.join();

    std::cout << "Test is done!\n";
    return 0;
}
