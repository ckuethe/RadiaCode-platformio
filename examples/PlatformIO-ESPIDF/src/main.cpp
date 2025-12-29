/**
 * RadiaCode ESP-IDF Example
 * 
 * This example demonstrates how to use the RadiaCode library with ESP-IDF framework.
 * The library automatically uses esp-nimble-cpp for BLE communication when using ESP-IDF.
 * 
 * Hardware: ESP32 development board
 * Framework: ESP-IDF via PlatformIO
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

// RadiaCode library includes (C++ code wrapped in extern "C" for ESP-IDF)
#ifdef __cplusplus
extern "C" {
#endif

void app_main(void);

#ifdef __cplusplus
}
#endif

// Include RadiaCode library
#include "RadiaCode.h"

// Replace with your device's MAC address
const char* bluetoothMac = "11:22:33:44:55:66"; 

extern "C" void app_main(void)
{
    // Initialize NVS (required for BLE)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }
    
    printf("\n=== RadiaCode ESP-IDF Example ===\n");
    printf("Connecting to RadiaCode device...\n");
    
    // Create RadiaCode instance
    RadiaCode* radiacode = new RadiaCode(bluetoothMac);
    
    if (radiacode != nullptr)
    {
        // Get device serial number to verify connection
        String serialNum = radiacode->serialNumber();
        if (serialNum.length() > 0)
        {
            printf("Connected to device: %s\n", serialNum.c_str());
            
            // Main loop - read radiation data
            while (true)
            {
                // Read radiation data
                std::vector<DataItem*> data = radiacode->dataBuf();
                
                // Process data
                for (DataItem* item : data)
                {
                    // Check type without dynamic_cast for efficiency
                    if (item->type == TYPE_REAL_TIME_DATA)
                    {
                        RealTimeData* rtData = static_cast<RealTimeData*>(item);
                        printf("Count rate: %.2f CPS\n", rtData->count_rate);
                        printf("Dose rate: %.2f µSv/h\n", rtData->dose_rate * 10000.0f);
                    }
                }
                
                // Clean up data objects
                for (DataItem* item : data)
                {
                    delete item;
                }
                data.clear();
                
                // Wait before next reading
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
        else
        {
            printf("Failed to connect to RadiaCode device\n");
        }
    }
    else
    {
        printf("Failed to create RadiaCode instance\n");
    }
    
    // Clean up
    delete radiacode;
}
