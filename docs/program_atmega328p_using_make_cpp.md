Yes! You can program an **Arduino Nano (ATmega328P)** using C++ and the command line **without the Arduino IDE**. Here’s how:

---

## **1. Install Required Tools**
You need the following command-line tools:

### **🛠 AVR Toolchain**
- Install **AVR-GCC** (compiler) and **AVRDUDE** (uploader):

#### **🔹 On Ubuntu/Debian:**
```sh
sudo apt update
sudo apt install avr-gcc avr-libc avrdude
```

#### **🔹 On Windows (via MSYS2 or Chocolatey):**
```sh
choco install avrdude avr-gcc
```
Or use **WinAVR** ([Download](https://sourceforge.net/projects/winavr/)).

---

## **2. Write a C++ Program**
Create a file called `main.cpp`:
```cpp
#include <avr/io.h>
#include <util/delay.h>

int main() {
    DDRB |= (1 << PB5);  // Set pin 13 (PB5) as output

    while (true) {
        PORTB ^= (1 << PB5);  // Toggle LED
        _delay_ms(500);  // Wait 500ms
    }

    return 0;
}
```
This **blinks the built-in LED** (Pin 13) on the Nano.

---

## **3. Compile the Code**
Use `avr-g++` to compile:
```sh
avr-g++ -mmcu=atmega328p -Os -o main.elf main.cpp
```
Then generate a HEX file for flashing:
```sh
avr-objcopy -O ihex main.elf main.hex
```

---

## **4. Upload to Arduino Nano**
Connect the **Arduino Nano** via USB and upload using `avrdude`:

```sh
avrdude -c arduino -p m328p -P /dev/ttyUSB0 -b 115200 -U flash:w:main.hex:i
```
🔹 Replace `/dev/ttyUSB0` with the correct serial port:
- **Linux**: Run `ls /dev/ttyUSB*` or `ls /dev/ttyACM*`
- **Windows**: Find the COM port in **Device Manager** (e.g., `COM3`):
  ```sh
  avrdude -c arduino -p m328p -P COM3 -b 115200 -U flash:w:main.hex:i
  ```

---

## **5. Automate with a Makefile (Optional)**
To simplify the process, create a `Makefile`:
```make
MCU=atmega328p
PORT=/dev/ttyUSB0
BAUD=115200
CXX=avr-g++
OBJCOPY=avr-objcopy
AVRDUDE=avrdude

all: main.hex

main.elf: main.cpp
	$(CXX) -mmcu=$(MCU) -Os -o main.elf main.cpp

main.hex: main.elf
	$(OBJCOPY) -O ihex main.elf main.hex

upload: main.hex
	$(AVRDUDE) -c arduino -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:main.hex:i

clean:
	rm -f main.elf main.hex
```
Then, compile and upload with:
```sh
make && make upload
```

---

## **✅ Summary**
✔ **Write C++ Code** with `avr/io.h`  
✔ **Compile** using `avr-g++`  
✔ **Upload** with `avrdude`  
✔ **Automate with Makefile**  

This method lets you program an **Arduino Nano like a raw AVR microcontroller** without the Arduino IDE. 🚀 Let me know if you need more help!