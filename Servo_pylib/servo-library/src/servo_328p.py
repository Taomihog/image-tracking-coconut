class Servo_328P:
    def __init__(self, device_id, min_angle, max_angle):
        self.device_id = device_id
        self.min_angle = min_angle
        self.max_angle = max_angle
        self.enabled = False

    def enable(self):
        self.enabled = True
        print(f"Servo {self.device_id} enabled.")

    def rotate_to(self, fraction):
        if not self.enabled:
            raise Exception("Servo not enabled.")
        angle = self.min_angle + (self.max_angle - self.min_angle) * fraction
        print(f"Servo {self.device_id} rotating to {angle:.2f} degrees.")