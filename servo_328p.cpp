#include <string_view>
#include "servo_328p.hpp"

static int Servo_328P::n_dev = 0; // Initialize static member variable for the number of instances created
static int Servo_328P::serial_fd = -1; // Initialize static member variable for serial file descriptor
int Servo_328P::dev = 0; // Initialize static member variable for device ID

Servo_328P::Servo_328P(int dev_in) : Servo_base(dev_in) { 
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety when initializing the serial port
    ++n_dev; // Increment the static counter for the number of instances created
    if (n_dev > 1) {
        return; // Only the first instance need to open the serial port. Subsequent instances will return early to avoid multiple openings of the same serial port.
    }
    if(serial_fd != -1) {
        std::cerr << "Warning: Attempting to initialize Servo_328P while serial_fd is already open. This may lead to unexpected behavior.\n";
        return; // serial fd already opened
    }
    serial_fd = open_serial();
    if (serial_fd == -1) {
        std::cerr << "Failed to open serial port for Servo_328P\n";
    } else {
        std::cout << "Servo_328P initialized successfully on device " << dev << "\n";
    }
}

Servo_328P::~Servo_328P() {
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety when closing the serial port
    --n_dev;
    if (n_dev != 0) {
        return; // Only close the serial port if this is the last instance being destroyed. This prevents closing the serial port while other instances are still using it.
    }
    if (serial_fd == -1) { 
        std::cerr << "No more Servo_328P instances, but serial_fd is already closed.\n";
    } else if (close(serial_fd) == -1) {
        std::cerr << "Failed to close serial port for Servo_328P\n";
    } else {
        std::cout << "Serial port closed for Servo_328P\n";
    }
}

double Servo_328P::Rotate_to(double fraction) {
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety when accessing the serial port
    if (fraction < 0) fraction = 0;
    if (fraction > 1) fraction = 1;
    uint16_t value = static_cast<uint16_t>(fraction * 1000); 
    unsigned int message = (dev << 28) | (value & 0xFFFF);
    // Close the serial port
    int try_count = 0;
    while (write(serial_fd, &message, sizeof(message)) != sizeof(message) && try_count < 3) {
        ++try_count;
        std::cerr << "Error writing to serial port, attempt " << try_count << " of 3\n";
        usleep(100000);
    }
    if (try_count == 3) {
        std::cerr << "Failed to send message after 3 attempts\n";
        close(serial_fd); 
        serial_fd = open_serial();
        if (serial_fd == -1) {
            std::cerr << "Failed to reopen serial port\n";
            return -1;
        } else {
            std::cout << "Serial port reopened successfully\n";
        }
    }
    return fraction;
}

int Servo_328P::open_serial() {
    constexpr string_view port = "/dev/ttyUSB0";  // Serial port
    int baudrate = B115200;             // Baud rate

    // Open serial port
    int serial_fd = open(port.data(), O_RDWR | O_NOCTTY);
    if (serial_fd == -1) {
        std::cerr << "Error opening serial port!\n";
        return 1;
    }

    // Configure the serial port
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(serial_fd, &tty) != 0) {
        std::cerr << "Error getting attributes!\n";
        return 1;
    }

    // Set baud rate
    cfsetospeed(&tty, baudrate);
    cfsetispeed(&tty, baudrate);

    // 8N1 mode (8 data bits, no parity, 1 stop bit)
    tty.c_cflag = CS8 | CLOCAL | CREAD;

    // Disable hardware flow control
    tty.c_cflag &= ~CRTSCTS;

    // Disable software flow control
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Raw mode (no processing of input/output)
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_oflag &= ~OPOST;

    // Set timeout (timeout = 1s)
    tty.c_cc[VMIN] = 0;  // Minimum number of characters to read
    tty.c_cc[VTIME] = 10; // Timeout in 1/10 seconds (1s)

    // Apply settings
    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting attributes!\n";
        return -1;
    }

    std::cout << "Serial port " << port.data() << " opened at " << 115200 << " baud\n";
    return serial_fd; // Return the file descriptor for the opened serial port
}
