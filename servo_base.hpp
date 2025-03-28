#pragma once

class Servo_base {
public:
    Servo_base(int dev) : dev(dev) {};
    ~Servo_base();
    virtual bool Enable() { return true; };
    virtual bool Disable() { return true; };
    virtual double Rotate_to(double fraction) = 0;  // Rotate the servo to a specified position as a fraction of the full range (0.0 to 1.0)
protected:
    int dev;
};


