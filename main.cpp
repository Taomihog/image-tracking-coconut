#include <iostream>
#include "servo_rpi.hpp"
#include "servo_328p.hpp"

void test(Servo* servos){
    while (true) {
        for (int i = 0; i <= 100; i += 10) {
            for(auto servo : servos) { // Iterate over each servo in the array
                double fraction = i / 100.0; // Calculate the fraction for rotation (0.0 to 1.0)
                if (servo) {
                    servo->Rotate_to(fraction); // Rotate the servo to the specified fraction
                    std::cout << "Rotated to " << fraction * 100 << "%\n";
                } else {
                    std::cerr << "Servo pointer is null\n";
                }
                usleep(50000); // Sleep for 50ms to allow the servo to reach the position before moving to the next one
            }
        }
        for (int i = 100; i >= 0; --i) {
            for(auto servo : servos) { // Iterate over each servo in the array
                double fraction = i / 100.0; // Calculate the fraction for rotation (0.0 to 1.0)
                if (servo) {
                    servo->Rotate_to(fraction); // Rotate the servo to the specified fraction
                    std::cout << "Rotated to " << fraction * 100 << "%\n";
                } else {
                    std::cerr << "Servo pointer is null\n";
                }
                usleep(50000); // Sleep for 50ms to allow the servo to reach the position before moving to the next one
            }
        }
    }
}

int main() {
    Servo_328P left_wheel(0); // Create an instance for the left wheel servo on device 0
    Servo_328P right_wheel(1); // Create an instance for the right wheel servo on device 1
    Servo_rpi cam_vertical(0); // Create an instance for the camera vertical servo on PWM device 0 (Raspberry Pi)
    Servo_rpi cam_horizontal(1); // Create an instance for the camera horizontal servo on PWM device 1 (Raspberry Pi)
    Servo_base* servos[] = { &left_wheel, &right_wheel, &cam_vertical, &cam_horizontal };
    
    // test these servos
    test(servos);
    return 0;
}