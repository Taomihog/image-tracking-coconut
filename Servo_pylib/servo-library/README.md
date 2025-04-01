# Servo Library

This project provides Python class libraries for controlling servo motors on two different platforms: the 328P and Raspberry Pi. 

## Project Structure

The project is organized as follows:

```
servo-library
├── src
│   ├── servo_328p.py        # Class for 328P platform servo control
│   ├── servo_rpi.py         # Class for Raspberry Pi servo control
│   └── __init__.py          # Package initialization
├── tests
│   ├── test_servo_328p.py   # Unit tests for Servo_328P
│   ├── test_servo_rpi.py    # Unit tests for Servo_rpi
│   └── __init__.py          # Package initialization for tests
├── requirements.txt          # Project dependencies
├── setup.py                  # Package configuration
└── README.md                 # Project documentation
```

## Installation

To install the libraries, follow these steps:

1. Navigate to the project directory:
   ```
   cd servo-library
   ```

2. Install the required dependencies:
   ```
   pip install -r requirements.txt
   ```

3. Build the package:
   ```
   python setup.py sdist bdist_wheel
   ```

4. Install the package locally (optional):
   ```
   pip install .
   ```

## Usage

After installation, you can use the libraries in your Python projects. Here’s a brief overview of how to use each class:

### Servo_328P

To control a servo motor on the 328P platform, you can create an instance of the `Servo_328P` class and call its methods to enable the servo and rotate it to a specified position.

### Servo_rpi

For controlling a servo motor on the Raspberry Pi, create an instance of the `Servo_rpi` class and use its methods similarly to control the servo.

## Running Tests

To ensure everything is working correctly, run the tests included in the project:

```
pytest tests/
```

This will execute all unit tests and provide feedback on the functionality of the servo classes.