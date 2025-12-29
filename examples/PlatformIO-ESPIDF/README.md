# RadiaCode ESP-IDF Example

This example demonstrates how to use the RadiaCode library with the ESP-IDF framework through PlatformIO.

## Overview

The RadiaCode library now supports both Arduino and ESP-IDF frameworks:

- **Arduino Framework**: Uses the Arduino BLE library
- **ESP-IDF Framework**: Uses the esp-nimble-cpp library

The library automatically detects which framework is being used and includes the appropriate BLE implementation.

## Requirements

- PlatformIO Core or PlatformIO IDE
- ESP32 development board
- RadiaCode radiation detector (102, 103, 103G, or 110)

## Building and Uploading

### ESP-IDF Framework (Default)

```bash
# Build the project
pio run -e espidf-esp32

# Upload to ESP32
pio run -e espidf-esp32 -t upload

# Monitor serial output
pio device monitor -e espidf-esp32
```

### Arduino Framework (For Comparison)

```bash
# Build with Arduino framework
pio run -e arduino-esp32

# Upload to ESP32
pio run -e arduino-esp32 -t upload
```

## Configuration

Before building, update the MAC address in `src/main.cpp`:

```cpp
const char* bluetoothMac = "11:22:33:44:55:66";  // Replace with your device's MAC
```

## How It Works

1. **Component Detection**: The library uses preprocessor macros to detect the framework:
   - `ARDUINO_ARCH_ESP32` for Arduino
   - `ESP_PLATFORM` for ESP-IDF

2. **BLE Library Selection**:
   - Arduino: `#include <BLEDevice.h>` from Arduino ESP32
   - ESP-IDF: `#include <NimBLEDevice.h>` from esp-nimble-cpp

3. **API Compatibility**: The esp-nimble-cpp library provides compatibility aliases (`BLEDevice`, `BLEClient`, etc.) that match the Arduino BLE API, allowing the same code to work on both frameworks with minimal changes.

## Project Structure

```
PlatformIO-ESPIDF/
├── platformio.ini      # PlatformIO configuration
├── src/
│   └── main.cpp        # ESP-IDF example code
└── README.md           # This file
```

## Dependencies

The ESP-IDF build automatically includes:
- `h2zero/esp-nimble-cpp` - NimBLE BLE stack wrapper (declared in `idf_component.yml`)

## Troubleshooting

### Build Issues

If you encounter build errors:

1. Clean the build: `pio run -t clean`
2. Update PlatformIO: `pio upgrade`
3. Update platform: `pio platform update espressif32`

### Connection Issues

If the device fails to connect:

1. Verify the MAC address is correct
2. Ensure the RadiaCode device is powered on and in range
3. Check that Bluetooth is not already connected to another device

## License

MIT License - Same as the RadiaCode library
