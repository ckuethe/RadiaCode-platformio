/**
 * ============================================================================
 * RadiaCode ESP-IDF Example for PlatformIO
 * ============================================================================
 * 
 * This example demonstrates how to use the RadiaCode library with ESP-IDF.
 * Perfect for beginners learning ESP32 and radiation detection!
 * 
 * What this does:
 * 1. Connects to a RadiaCode device via Bluetooth Low Energy (BLE)
 * 2. Reads real-time radiation measurements
 * 3. Displays count rate (CPS) and dose rate (µSv/h)
 * 
 * Hardware Required:
 * - ESP32 development board (any variant)
 * - RadiaCode radiation detector (102, 103, 103G, or 110)
 * - USB cable for programming and power
 * 
 * Before You Begin:
 * 1. Find your RadiaCode's Bluetooth MAC address (see README.md)
 * 2. Update the 'bluetoothMac' variable below with your device's address
 * 3. Build and upload to your ESP32
 * 
 * Framework: ESP-IDF with Arduino as component
 * Libraries: RadiaCode, esp-nimble-cpp
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

// RadiaCode library includes
// Note: C++ code needs to be wrapped in extern "C" for ESP-IDF
#ifdef __cplusplus
extern "C" {
#endif

void app_main(void);

#ifdef __cplusplus
}
#endif

#include "RadiaCode.h"

// ============================================================================
// CONFIGURATION - CHANGE THIS!
// ============================================================================

// ** IMPORTANT: Replace with YOUR RadiaCode device's MAC address **
// Format: "AA:BB:CC:DD:EE:FF" (six pairs of hex digits separated by colons)
// 
// How to find your MAC address:
// - Android: Settings → Bluetooth → Device settings
// - iOS: Use a BLE scanner app
// - ESP32: Run BLE scan code (see README.md)
const char* bluetoothMac = "11:22:33:44:55:66"; 

// ============================================================================
// MAIN APPLICATION
// ============================================================================

extern "C" void app_main(void)
{
    // ------------------------------------------------------------------------
    // Step 1: Initialize NVS (Non-Volatile Storage)
    // ------------------------------------------------------------------------
    // NVS is required for BLE to store pairing information
    // This initializes the flash memory partition for NVS
    printf("\n");
    printf("============================================\n");
    printf("  RadiaCode ESP-IDF Example\n");
    printf("============================================\n\n");
    
    printf("[1/4] Initializing NVS...\n");
    esp_err_t ret = nvs_flash_init();
    
    // If NVS partition is full or corrupted, erase and retry
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("      NVS partition needs erase, erasing...\n");
        nvs_flash_erase();
        ret = nvs_flash_init();
    }
    
    if (ret == ESP_OK) {
        printf("      NVS initialized successfully\n\n");
    } else {
        printf("      ERROR: Failed to initialize NVS (code: %d)\n", ret);
        return;
    }
    
    // ------------------------------------------------------------------------
    // Step 2: Connect to RadiaCode Device
    // ------------------------------------------------------------------------
    printf("[2/4] Connecting to RadiaCode device...\n");
    printf("      MAC Address: %s\n", bluetoothMac);
    printf("      This may take 10-30 seconds...\n");
    
    // Create RadiaCode instance - this initiates BLE connection
    // The constructor will:
    // 1. Initialize BLE stack (NimBLE)
    // 2. Create a BLE client
    // 3. Connect to the device at the specified MAC address
    // 4. Discover services and characteristics
    // 5. Enable notifications for data reception
    RadiaCode* radiacode = new RadiaCode(bluetoothMac);
    
    // Check if instance was created successfully
    if (radiacode == nullptr)
    {
        printf("      ERROR: Failed to create RadiaCode instance\n");
        printf("      Possible reasons:\n");
        printf("      - Out of memory\n");
        printf("      - BLE stack initialization failed\n");
        return;
    }
    
    // ------------------------------------------------------------------------
    // Step 3: Verify Connection
    // ------------------------------------------------------------------------
    printf("\n[3/4] Verifying connection...\n");
    
    // Try to read the device serial number
    // This confirms that:
    // 1. BLE connection is established
    // 2. We can communicate with the device
    // 3. The device is responding
    String serialNum = radiacode->serialNumber();
    
    if (serialNum.length() > 0)
    {
        // Success! We're connected and can communicate
        printf("      ✓ Connected successfully!\n");
        printf("      Device: %s\n", serialNum.c_str());
        
        // Optionally get more device info
        printf("\n      Device Information:\n");
        printf("      -------------------\n");
        
        // You can also get:
        // - Firmware version: radiacode->fwSignature()
        // - Hardware serial: radiacode->hwSerialNumber()
        // - Temperature: radiacode->getTemperature()
        // - Configuration: radiacode->configuration()
        
        printf("      Serial Number: %s\n", serialNum.c_str());
        printf("\n");
        
        // ------------------------------------------------------------------------
        // Step 4: Main Loop - Read and Display Data
        // ------------------------------------------------------------------------
        printf("[4/4] Reading radiation data...\n");
        printf("      Press Ctrl+C to stop\n\n");
        printf("============================================\n\n");
        
        // Infinite loop to continuously read data
        int readingCount = 0;
        
        while (true)
        {
            // Get radiation data from device
            // dataBuf() returns a vector of DataItem pointers
            // The device may return multiple types of data:
            // - Real-time data (count rate, dose rate)
            // - Spectrum data (energy distribution)
            // - Device status
            std::vector<DataItem*> data = radiacode->dataBuf();
            
            // Process each data item received
            for (DataItem* item : data)
            {
                // Check the type of data we received
                // We're interested in real-time radiation measurements
                if (item->type == TYPE_REAL_TIME_DATA)
                {
                    // Cast to RealTimeData to access radiation values
                    RealTimeData* rtData = static_cast<RealTimeData*>(item);
                    
                    readingCount++;
                    
                    // Display the radiation measurements
                    printf("Reading #%d:\n", readingCount);
                    printf("  Count Rate: %.2f CPS (counts per second)\n", 
                           rtData->count_rate);
                    
                    // Convert dose rate to µSv/h (microsieverts per hour)
                    // The raw value is in different units, so we multiply by 10000
                    float doseMicroSvPerHour = rtData->dose_rate * 10000.0f;
                    printf("  Dose Rate:  %.3f µSv/h (microsieverts per hour)\n", 
                           doseMicroSvPerHour);
                    
                    // Optional: display accumulated dose
                    // printf("  Accumulated: %.3f µSv\n", rtData->accumulated_dose * 10000.0f);
                    
                    printf("\n");
                }
                
                // You can also process other data types:
                // - TYPE_SPECTRUM: Energy spectrum data for analysis
                // - TYPE_DOSE_RATE: More detailed dose rate info
                // See RadiaCodeTypes.h for all available types
            }
            
            // Clean up: Delete all data items to free memory
            // Very important to prevent memory leaks!
            for (DataItem* item : data)
            {
                delete item;
            }
            data.clear();
            
            // Wait 1 second before next reading
            // Using FreeRTOS delay (not regular delay)
            // pdMS_TO_TICKS converts milliseconds to FreeRTOS ticks
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    else
    {
        // Connection failed
        printf("      ✗ Failed to connect to device\n\n");
        printf("Troubleshooting:\n");
        printf("----------------\n");
        printf("1. Check MAC address is correct: %s\n", bluetoothMac);
        printf("   - Format should be: AA:BB:CC:DD:EE:FF\n");
        printf("   - All letters should be uppercase or lowercase (not mixed)\n\n");
        
        printf("2. Ensure RadiaCode device is:\n");
        printf("   - Powered on\n");
        printf("   - Within 2-3 meters of ESP32\n");
        printf("   - Not connected to another device (phone, tablet)\n");
        printf("   - Bluetooth is enabled\n\n");
        
        printf("3. Try:\n");
        printf("   - Restarting both devices\n");
        printf("   - Moving ESP32 closer to RadiaCode\n");
        printf("   - Running BLE scan to verify device is visible\n\n");
    }
    
    // Clean up: Free memory allocated for RadiaCode instance
    // This will also disconnect from the BLE device
    delete radiacode;
    
    printf("============================================\n");
    printf("Application ended\n");
}

// ============================================================================
// END OF MAIN APPLICATION
// ============================================================================

/**
 * Additional Notes:
 * 
 * 1. Memory Management:
 *    - Always delete DataItem objects after processing
 *    - The RadiaCode object allocates memory dynamically
 *    - Free it when done with 'delete radiacode'
 * 
 * 2. BLE Connection:
 *    - Initial connection can take 10-30 seconds
 *    - Device must not be connected to other devices
 *    - Some ESP32 boards may need external antenna for better range
 * 
 * 3. Error Handling:
 *    - Check serialNum length to verify connection
 *    - Monitor for empty data vectors (device may be disconnected)
 *    - Implement reconnection logic for production code
 * 
 * 4. Power Consumption:
 *    - BLE is relatively power-efficient
 *    - Can run on battery power
 *    - Consider deep sleep between readings for ultra-low power
 * 
 * 5. Next Steps:
 *    - Add WiFi to send data to cloud/server
 *    - Add display (OLED/LCD) to show readings
 *    - Implement data logging to SD card
 *    - Create web interface for remote monitoring
 *    - Analyze spectrum data for isotope identification
 * 
 * For more information, see:
 * - README.md in this directory
 * - RadiaCode library documentation
 * - ESP-IDF programming guide
 */
