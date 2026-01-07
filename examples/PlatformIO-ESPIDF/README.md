# RadiaCode ESP-IDF Example for PlatformIO

This is a complete, beginner-friendly example demonstrating how to use the RadiaCode library with ESP-IDF framework in PlatformIO.

## 📋 What You'll Learn

- How to set up a PlatformIO project for ESP-IDF
- How to add the RadiaCode library as a dependency
- How to configure and build an ESP-IDF project
- How to connect to and read data from a RadiaCode device

## 🎯 Prerequisites

Before you start, you'll need:

### Hardware
- **ESP32 development board** (ESP32-DevKitC, NodeMCU-32S, or similar)
- **USB cable** to connect ESP32 to your computer
- **RadiaCode radiation detector** (models 102, 103, 103G, or 110)

### Software
- **Python 3.7+** installed on your system
- **PlatformIO Core** or **PlatformIO IDE** (VS Code extension)

### Installing PlatformIO

Choose one of these options:

#### Option 1: PlatformIO IDE (Recommended for Beginners)
1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Open VS Code
3. Go to Extensions (Ctrl+Shift+X or Cmd+Shift+X)
4. Search for "PlatformIO IDE"
5. Click Install

#### Option 2: PlatformIO Core (Command Line)
```bash
# Install using pip
pip install platformio

# Verify installation
pio --version
```

## 🚀 Quick Start Guide

### Step 1: Create a New Project

#### Using PlatformIO IDE (VS Code):
1. Open VS Code with PlatformIO installed
2. Click on PlatformIO icon in the sidebar
3. Click "New Project"
4. Configure:
   - **Name**: RadiaCode-ESP-IDF-Demo
   - **Board**: Select your ESP32 board (e.g., "Espressif ESP32 Dev Module")
   - **Framework**: Select "Espidf"
5. Click "Finish"

#### Using Command Line:
```bash
# Create new project
mkdir RadiaCode-ESP-IDF-Demo
cd RadiaCode-ESP-IDF-Demo
pio project init --board esp32dev --project-option "framework=espidf"
```

### Step 2: Add the RadiaCode Library

You have **three options** to add the RadiaCode library to your project:

#### Option A: Using Git Repository (Recommended)

Edit your `platformio.ini` file:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = espidf, arduino
monitor_speed = 115200

lib_deps = 
    https://github.com/ckuethe/RadiaCode-platformio.git
    h2zero/esp-nimble-cpp@^2.0.0
```

#### Option B: Using Local Path

If you have the library downloaded locally:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = espidf, arduino
monitor_speed = 115200

lib_deps = 
    file:///path/to/RadiaCode-platformio
    h2zero/esp-nimble-cpp@^2.0.0
```

Replace `/path/to/RadiaCode-platformio` with the actual path to the library folder.

#### Option C: Copy Library to lib/ Folder

1. Download/clone the RadiaCode library
2. Copy the entire library folder to `lib/RadiaCode` in your project
3. Update `platformio.ini`:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = espidf, arduino
monitor_speed = 115200

lib_deps = 
    h2zero/esp-nimble-cpp@^2.0.0
```

### Step 3: Copy the Example Code

Create or edit `src/main.cpp` with the example code from this directory, or use the simplified version below.

### Step 4: Find Your RadiaCode MAC Address

You need the Bluetooth MAC address of your RadiaCode device:

#### On Android:
1. Open Settings → Bluetooth
2. Find your RadiaCode device in the list
3. Tap the settings icon next to it
4. Note the MAC address (format: `AA:BB:CC:DD:EE:FF`)

#### On iOS:
iOS doesn't show MAC addresses directly. You can:
1. Use a BLE scanner app from the App Store
2. Or use the ESP32 to scan for devices (see Troubleshooting section)

#### On Linux:
```bash
bluetoothctl
scan on
# Look for "RadiaCode" in the list
```

### Step 5: Update the Code

Edit `src/main.cpp` and replace the MAC address:

```cpp
// Replace with YOUR device's MAC address
const char* bluetoothMac = "11:22:33:44:55:66";
```

### Step 6: Build and Upload

### Step 6: Build and Upload

#### Using PlatformIO IDE:
1. Connect your ESP32 to your computer via USB
2. In VS Code, open PlatformIO menu (checkmark icon at bottom)
3. Click "Build" to compile the project
4. Once build succeeds, click "Upload" to flash to ESP32
5. Click "Monitor" to see serial output

#### Using Command Line:
```bash
# Build the project
pio run

# Upload to ESP32 (make sure it's connected)
pio run --target upload

# Open serial monitor to see output
pio device monitor
```

### Step 7: View the Output

Once uploaded, you should see output like this in the serial monitor:

```
=== RadiaCode ESP-IDF Example ===
Connecting to RadiaCode device...
Connected to device: RC-103-12345
Count rate: 15.23 CPS
Dose rate: 0.08 µSv/h
Count rate: 14.87 CPS
Dose rate: 0.08 µSv/h
...
```

## 📁 Complete Project Structure

Your project should look like this:

```
RadiaCode-ESP-IDF-Demo/
├── platformio.ini          # Project configuration
├── src/
│   └── main.cpp           # Your application code
├── lib/                   # Optional: local libraries
├── include/               # Optional: header files
└── .pio/                  # Build files (auto-generated)
```

## 🔧 Understanding the Configuration

### platformio.ini Explained

```ini
[env:esp32dev]
platform = espressif32      # ESP32 platform
board = esp32dev            # Your ESP32 board type
framework = espidf, arduino # Use BOTH ESP-IDF and Arduino
monitor_speed = 115200      # Serial monitor baud rate

lib_deps = 
    # RadiaCode library (choose one method from Step 2)
    https://github.com/ckuethe/RadiaCode-platformio.git
    # BLE library for ESP-IDF
    h2zero/esp-nimble-cpp@^2.0.0
```

### Why Both Frameworks?

The RadiaCode library uses Arduino APIs (like `String` and `Serial`) for simplicity. By specifying `framework = espidf, arduino`, PlatformIO:
1. Uses ESP-IDF as the main framework
2. Includes Arduino as an ESP-IDF component
3. Gives you access to both ESP-IDF features AND Arduino convenience functions

### About Dependencies

- **RadiaCode library**: Provides the interface to communicate with RadiaCode devices
- **esp-nimble-cpp**: A lightweight BLE stack for ESP32 that works with ESP-IDF

## 💻 Example Code Explained

Here's a simplified, well-commented example:

```cpp
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "RadiaCode.h"

// IMPORTANT: Replace with your device's MAC address
const char* bluetoothMac = "11:22:33:44:55:66";

extern "C" void app_main(void)
{
    // Initialize NVS (Non-Volatile Storage) - required for BLE
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }
    
    printf("\n=== RadiaCode ESP-IDF Example ===\n");
    printf("Connecting to RadiaCode device...\n");
    
    // Create RadiaCode object and connect via Bluetooth
    RadiaCode* radiacode = new RadiaCode(bluetoothMac);
    
    if (radiacode != nullptr)
    {
        // Try to get serial number to verify connection
        String serialNum = radiacode->serialNumber();
        if (serialNum.length() > 0)
        {
            printf("Connected to device: %s\n", serialNum.c_str());
            
            // Main loop - continuously read radiation data
            while (true)
            {
                // Get data from device
                std::vector<DataItem*> data = radiacode->dataBuf();
                
                // Process each data item
                for (DataItem* item : data)
                {
                    if (item->type == TYPE_REAL_TIME_DATA)
                    {
                        RealTimeData* rtData = static_cast<RealTimeData*>(item);
                        
                        // Display radiation readings
                        printf("Count rate: %.2f CPS\n", rtData->count_rate);
                        printf("Dose rate: %.2f µSv/h\n", rtData->dose_rate * 10000.0f);
                    }
                }
                
                // Clean up
                for (DataItem* item : data)
                {
                    delete item;
                }
                data.clear();
                
                // Wait 1 second before next reading
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
        else
        {
            printf("Failed to connect to RadiaCode device\n");
            printf("Check MAC address and device is powered on\n");
        }
    }
    else
    {
        printf("Failed to create RadiaCode instance\n");
    }
    
    delete radiacode;
}
```

### Key Functions:

- **`RadiaCode(mac)`**: Creates connection to device
- **`serialNumber()`**: Gets device serial number (verifies connection)
- **`dataBuf()`**: Retrieves real-time radiation data
- **`count_rate`**: Counts per second (CPS)
- **`dose_rate`**: Radiation dose rate (multiply by 10000 for µSv/h)

## 🔍 Troubleshooting

### "Failed to connect to BLE device"

**Problem**: Can't connect to RadiaCode device

**Solutions**:
1. **Check MAC address**: Make sure it's correct and in format `AA:BB:CC:DD:EE:FF`
2. **Device powered on**: Ensure RadiaCode is turned on and not connected to another device
3. **Bluetooth range**: Move ESP32 closer to RadiaCode (within 2-3 meters)
4. **Scan for devices**: Use this code to find available BLE devices:

```cpp
#include <NimBLEDevice.h>

void scanBLE() {
    NimBLEDevice::init("");
    NimBLEScan* pScan = NimBLEDevice::getScan();
    pScan->setActiveScan(true);
    
    printf("Scanning for BLE devices...\n");
    NimBLEScanResults results = pScan->start(5);
    
    for (int i = 0; i < results.getCount(); i++) {
        NimBLEAdvertisedDevice device = results.getDevice(i);
        printf("Found: %s - %s\n", 
               device.getName().c_str(), 
               device.getAddress().toString().c_str());
    }
}
```

### Build Errors

**Problem**: Compilation fails

**Solutions**:
1. **Clean build**: `pio run -t clean` then rebuild
2. **Update platform**: `pio platform update espressif32`
3. **Check framework**: Ensure `platformio.ini` has `framework = espidf, arduino`
4. **Dependencies**: Verify both RadiaCode and esp-nimble-cpp are in `lib_deps`

### Upload Fails

**Problem**: Can't upload to ESP32

**Solutions**:
1. **Check USB connection**: Try different USB cable/port
2. **Install drivers**: May need CP210x or CH340 drivers
3. **Specify port**: Add to `platformio.ini`:
   ```ini
   upload_port = /dev/ttyUSB0  # Linux
   upload_port = COM3           # Windows
   ```
4. **Hold BOOT button**: Some boards need BOOT button held during upload

### Monitor Shows Nothing

**Problem**: Serial monitor blank or garbage

**Solutions**:
1. **Check baud rate**: Must match `monitor_speed = 115200`
2. **Reset ESP32**: Press RST/EN button after upload
3. **Check USB connection**: Ensure data cable (not just power)

## 📊 What Data Can You Read?

The RadiaCode library provides access to:

### Real-Time Data
- **Count rate** (CPS - counts per second)
- **Dose rate** (µSv/h - microsieverts per hour)
- **Dose accumulated** (µSv)
- **Duration** (seconds)

### Spectrum Data
- **Energy spectrum**: 1024 channels of gamma energy data
- **Calibration coefficients**: For energy calculation
- **Duration**: Acquisition time

### Device Information
- **Serial number**
- **Firmware version**
- **Hardware version**
- **Battery level**
- **Temperature**

### Configuration
- **Alarm thresholds**: Set dose rate alarms
- **Settings**: Various device settings

See the full RadiaCode library documentation for all available functions.

## 🎓 Next Steps

Once you have this basic example working, you can:

1. **Add WiFi**: Send data to a server or MQTT broker
2. **Add display**: Show readings on OLED/LCD screen
3. **Data logging**: Save readings to SD card or SPIFFS
4. **Web interface**: Create a web dashboard
5. **Spectrum analysis**: Process and visualize gamma spectrum
6. **Multiple devices**: Connect to multiple RadiaCode devices

## 📚 Additional Resources

- [PlatformIO Documentation](https://docs.platformio.org/)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/)
- [RadiaCode Library Reference](../../README.md)
- [esp-nimble-cpp Documentation](https://github.com/h2zero/esp-nimble-cpp)

## 🐛 Getting Help

If you encounter issues:

1. Check this troubleshooting section
2. Review PlatformIO build output for specific errors
3. Open an issue on the [GitHub repository](https://github.com/ckuethe/RadiaCode-platformio/issues)
4. Include: ESP32 board type, PlatformIO version, complete error messages

## Building and Uploading

## ⚙️ Advanced: Using Native ESP-IDF Component Manager

For advanced users who want to use the library as a native ESP-IDF component (without PlatformIO), create a `main/idf_component.yml` in your ESP-IDF project:

```yaml
dependencies:
  ckuethe/RadiaCode-platformio:
    git: https://github.com/ckuethe/RadiaCode-platformio.git
    version: "*"
  h2zero/esp-nimble-cpp:
    version: "^2.0.0"
```

Then build with:
```bash
idf.py build
idf.py flash monitor
```

## 📝 License

MIT License - Same as the RadiaCode library
