class Servo_rpi:
    def __init__(self, pwm_channel, min_pulse_width=0.5, max_pulse_width=2.5):
        self.pwm_channel = pwm_channel
        self.min_pulse_width = min_pulse_width
        self.max_pulse_width = max_pulse_width
        self.enabled = False

    def enable(self):
        # Code to enable the servo (e.g., initialize PWM)
        self.enabled = True

    def rotate_to(self, fraction):
        if not self.enabled:
            raise RuntimeError("Servo not enabled")
        if fraction < 0.0 or fraction > 1.0:
            raise ValueError("Fraction must be between 0.0 and 1.0")
        pulse_width = self.min_pulse_width + (self.max_pulse_width - self.min_pulse_width) * fraction
        # Code to set the PWM signal to the calculated pulse width
        print(f"Rotating servo on channel {self.pwm_channel} to {fraction * 100}% (pulse width: {pulse_width})")