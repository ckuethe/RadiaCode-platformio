# Quick Start Guide - RadiaCode with ESP-IDF

**For complete beginners who just want to get started quickly!**

## What You Need

1. **ESP32 board** (any ESP32 development board)
2. **USB cable** to connect ESP32 to computer
3. **RadiaCode device** (radiation detector)
4. **Computer** with Windows, Mac, or Linux

## Step-by-Step Instructions

### 1. Install Software (One-Time Setup)

#### Install PlatformIO IDE (Easiest Method)

1. Download and install [Visual Studio Code](https://code.visualstudio.com/)
2. Open VS Code
3. Click the Extensions icon on the left (or press `Ctrl+Shift+X`)
4. Search for "PlatformIO IDE"
5. Click "Install"
6. Wait for installation to complete (may take a few minutes)
7. Restart VS Code

**✓ You're ready!** You now have everything needed to program ESP32.

### 2. Create Your Project

#### Method A: Using PlatformIO in VS Code

1. Open VS Code
2. Click the PlatformIO icon (alien head) on the left sidebar
3. Click "New Project"
4. Fill in:
   - **Name**: `RadiaCode-Demo` (or any name you like)
   - **Board**: Select your ESP32 board type
     - If unsure, choose "Espressif ESP32 Dev Module"
   - **Framework**: Select "Espidf"
5. Click "Finish" (this creates the project)

#### Method B: Using Command Line

```bash
mkdir RadiaCode-Demo
cd RadiaCode-Demo
pio project init --board esp32dev --project-option "framework=espidf"
```

### 3. Add RadiaCode Library

Open the file `platformio.ini` in your project and **replace everything** with this:

```ini
[platformio]
default_envs = espidf-esp32

[env:espidf-esp32]
platform = espressif32
board = esp32dev
framework = espidf, arduino
monitor_speed = 115200
lib_deps = 
    https://github.com/ckuethe/RadiaCode-platformio.git
    h2zero/esp-nimble-cpp@^2.0.0
```

**Save the file!** (Ctrl+S or Cmd+S)

### 4. Copy Example Code

1. Create a file `src/main.cpp` (if it doesn't exist)
2. Copy the code from `examples/PlatformIO-ESPIDF/src/main.cpp` in this repository
3. Or use this minimal version:

```cpp
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "RadiaCode.h"

// CHANGE THIS to your device's MAC address!
const char* bluetoothMac = "11:22:33:44:55:66";

extern "C" void app_main(void)
{
    // Initialize NVS (required for Bluetooth)
    nvs_flash_init();
    
    printf("\n=== RadiaCode ESP-IDF ===\n");
    printf("Connecting to device...\n");
    
    // Connect to RadiaCode
    RadiaCode* radiacode = new RadiaCode(bluetoothMac);
    
    if (radiacode != nullptr)
    {
        String serial = radiacode->serialNumber();
        if (serial.length() > 0)
        {
            printf("Connected: %s\n\n", serial.c_str());
            
            while (true)
            {
                std::vector<DataItem*> data = radiacode->dataBuf();
                
                for (DataItem* item : data)
                {
                    if (item->type == TYPE_REAL_TIME_DATA)
                    {
                        RealTimeData* rtData = static_cast<RealTimeData*>(item);
                        printf("Count: %.2f CPS | Dose: %.2f µSv/h\n", 
                               rtData->count_rate,
                               rtData->dose_rate * 10000.0f);
                    }
                }
                
                for (DataItem* item : data) delete item;
                data.clear();
                
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
        else
        {
            printf("Connection failed! Check MAC address.\n");
        }
    }
    
    delete radiacode;
}
```

### 5. Find Your RadiaCode MAC Address

**Android:**
1. Open Settings → Bluetooth
2. Find "RadiaCode" or "RC-..." in device list
3. Tap the settings/info icon
4. Write down the MAC address (example: `A4:C1:38:12:34:56`)

**iOS:**
Download a "BLE Scanner" app from App Store and scan for "RadiaCode"

**Windows:**
1. Open Settings → Bluetooth & devices
2. Click on your RadiaCode device
3. Look for "Address" or use a Bluetooth scanner app

### 6. Update the Code

In `src/main.cpp`, find this line:

```cpp
const char* bluetoothMac = "11:22:33:44:55:66";
```

Replace `11:22:33:44:55:66` with your actual MAC address (keep the quotes!).

**Example:**
```cpp
const char* bluetoothMac = "A4:C1:38:12:34:56";
```

**Save the file!**

### 7. Connect ESP32

1. Connect your ESP32 to computer with USB cable
2. Wait for drivers to install (Windows may need CH340/CP210x drivers)

**Need drivers?**
- Windows: Download from manufacturer's website
- Mac/Linux: Usually work automatically

### 8. Build and Upload

#### Using VS Code PlatformIO:

1. Look at the bottom of VS Code - you'll see icons
2. Click the **checkmark** icon (✓) to build
3. Wait for "SUCCESS" message
4. Click the **arrow** icon (→) to upload
5. Click the **plug** icon to open serial monitor

#### Using Command Line:

```bash
# Build
pio run

# Upload
pio run -t upload

# Open monitor
pio device monitor
```

### 9. See Results!

In the serial monitor, you should see:

```
=== RadiaCode ESP-IDF ===
Connecting to device...
Connected: RC-103-12345

Count: 15.23 CPS | Dose: 0.08 µSv/h
Count: 14.87 CPS | Dose: 0.08 µSv/h
Count: 16.01 CPS | Dose: 0.09 µSv/h
...
```

**🎉 Congratulations!** You're reading radiation data!

## Troubleshooting

### "Connection failed!"

**Problem**: Can't connect to RadiaCode

**Solutions**:
1. Check MAC address is correct (format: `AA:BB:CC:DD:EE:FF`)
2. Turn RadiaCode off and on
3. Make sure RadiaCode is NOT connected to your phone
4. Move ESP32 closer to RadiaCode (within 1 meter)
5. Try restarting ESP32 (press RST button)

### "Error: Upload failed"

**Problem**: Can't upload to ESP32

**Solutions**:
1. Check USB cable (needs to be a data cable, not just power)
2. Install CH340 or CP210x USB drivers
3. Try different USB port
4. Hold BOOT button while uploading (some boards need this)

### "Build failed" or "Compilation error"

**Problem**: Code won't compile

**Solutions**:
1. Make sure you saved `platformio.ini` and `main.cpp`
2. Check that MAC address has correct format with quotes
3. Try: `pio run -t clean` then build again
4. Update PlatformIO: `pio upgrade`

### Nothing in Serial Monitor

**Problem**: Monitor is blank

**Solutions**:
1. Press RST/EN button on ESP32
2. Check baud rate is 115200
3. Try different USB cable
4. Unplug and plug USB cable again

## What's Next?

Once you have it working, you can:

1. **Add WiFi**: Send data to the internet
2. **Add Display**: Show readings on OLED screen
3. **Log Data**: Save to SD card
4. **Create Alarms**: Alert when radiation is high
5. **Spectrum Analysis**: Analyze gamma ray energies

See the full `README.md` in this folder for advanced features!

## Need More Help?

1. Read the detailed [README.md](README.md) in this folder
2. Check [PlatformIO documentation](https://docs.platformio.org/)
3. Ask questions on [GitHub Issues](https://github.com/ckuethe/RadiaCode-platformio/issues)

**Good luck with your project! 🚀📡☢️**
