import unittest
from src.servo_rpi import Servo_rpi

class TestServoRpi(unittest.TestCase):
    def setUp(self):
        self.servo = Servo_rpi(1)

    def test_enable(self):
        self.servo.Enable()
        self.assertTrue(self.servo.is_enabled)

    def test_rotate_to(self):
        self.servo.Enable()
        self.servo.Rotate_to(0.5)
        self.assertEqual(self.servo.current_position, 0.5)

    def test_rotate_to_invalid(self):
        self.servo.Enable()
        with self.assertRaises(ValueError):
            self.servo.Rotate_to(1.5)  # Invalid position

if __name__ == '__main__':
    unittest.main()