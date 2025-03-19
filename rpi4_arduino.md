Here’s how you can achieve **4-channel PWM control** on an **Arduino** using a **Raspberry Pi** over **serial communication** (`pyserial`).  

---

## **Message Format (uint32_t)**
Each **32-bit** message follows this structure:  
```
| 4 bits  | 4 bits  | 8 bits   | 16 bits |
| PWM ID  | CmdType | Reserved | Value   |
```
- **Bits [31:28]** → **PWM Channel** (0–15, only 0–3 used).  
- **Bit [27:24]** → **Parameter Type** (`0 = period`, `1 = duty cycle`).  
- **Bits [23:16]** → **Reserved**.  
- **Bits [15:0]** → **Value** (`uint16_t` to set period or duty cycle).  
---

**Endianness?**

## **1️⃣ Arduino Code (PWM Receiver)**
This **Arduino sketch**:  
- Reads `uint32_t` from **serial**.  
- Decodes **channel, parameter type, and value**.  
- Updates PWM **period** or **duty cycle**.

```cpp
#include <Arduino.h>

#define PWM_COUNT 4
const int pwmPins[PWM_COUNT] = {3, 5, 6, 9}; // Select PWM-capable pins

struct PWMConfig {
    uint16_t period = 2000; // Default period (μs)
    uint16_t duty = 1000;   // Default duty cycle (μs)
};

PWMConfig pwmChannels[PWM_COUNT];

void applyPWM(int channel) {
    if (channel < PWM_COUNT) {
        analogWrite(pwmPins[channel], map(pwmChannels[channel].duty, 0, pwmChannels[channel].period, 0, 255));
    }
}

void processMessage(uint32_t message) {
    uint8_t pwmID = (message >> 28) & 0x0F; // Extract 4-bit PWM ID
    bool isDuty = (message >> 27) & 0x01;   // Extract 1-bit parameter type
    uint16_t value = message & 0xFFFF;      // Extract 16-bit value

    if (pwmID >= PWM_COUNT) return; // Ignore invalid IDs

    if (isDuty) {
        pwmChannels[pwmID].duty = value; 
    } else {
        pwmChannels[pwmID].period = value;
    }
    
    applyPWM(pwmID);
}

void setup() {
    Serial.begin(115200);
    for (int i = 0; i < PWM_COUNT; i++) {
        pinMode(pwmPins[i], OUTPUT);
        applyPWM(i);
    }
}

void loop() {
    if (Serial.available() >= 4) {
        uint32_t receivedData = 0;
        Serial.readBytes((char*)&receivedData, sizeof(receivedData));
        processMessage(receivedData);
    }
}
```

---

## **2️⃣ Raspberry Pi Code (PWM Controller)**
This Python script:  
- Connects to **Arduino over USB serial**.  
- Sends `uint32_t` commands to **set PWM period or duty cycle**.  

### **Python Code (`rpi_pwm_control.py`)**
```python
import serial
import struct
import time

# Open serial connection
arduino = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
time.sleep(2)  # Wait for Arduino to initialize

def send_pwm_command(pwm_id, is_duty, value):
    if pwm_id < 0 or pwm_id > 3:
        print("Invalid PWM ID")
        return
    
    message = (pwm_id << 28) | (is_duty << 27) | (value & 0xFFFF)
    arduino.write(struct.pack('<I', message))

# Example usage
send_pwm_command(0, 0, 2000)  # Set PWM0 period to 2000μs
time.sleep(0.1)
send_pwm_command(0, 1, 1000)  # Set PWM0 duty cycle to 1000μs
time.sleep(0.1)
send_pwm_command(1, 1, 1500)  # Set PWM1 duty cycle to 1500μs
```

---

## **💡 How It Works**
1. **Arduino**
   - Reads **4 bytes** (`uint32_t`) from the Raspberry Pi.  
   - Extracts **PWM channel**, **parameter type**, and **value**.  
   - Updates **period** or **duty cycle** and applies PWM.

2. **Raspberry Pi**
   - Sends a `uint32_t` command via **pyserial**.
   - `send_pwm_command(pwm_id, is_duty, value)`:
     - `pwm_id`: **Which PWM** (0–3).  
     - `is_duty`: **0 = period, 1 = duty cycle**.  
     - `value`: **16-bit parameter value**.  

---

### **✅ Features & Customizations**
- 🚀 **Fast Serial Communication (115200 baud)**.  
- 🔄 **Dynamic PWM Control** without modifying Arduino code.  
- 🔧 Can easily **add more PWM channels**.  
- 🎯 Works with any Arduino board supporting **PWM + Serial**.  

Would you like help **modifying for specific PWM frequencies** or adding **bi-directional communication**? 🚀
