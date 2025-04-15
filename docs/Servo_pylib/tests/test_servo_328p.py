import unittest
from src.servo_328p import Servo_328P

class TestServo328P(unittest.TestCase):

    def setUp(self):
        self.servo = Servo_328P(0, 0.28, 0.68)

    def test_initialization(self):
        self.assertIsNotNone(self.servo)
        self.assertEqual(self.servo.device_id, 0)
        self.assertEqual(self.servo.min_position, 0.28)
        self.assertEqual(self.servo.max_position, 0.68)

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
            self.servo.Rotate_to(1.5)  # Out of bounds

if __name__ == '__main__':
    unittest.main()