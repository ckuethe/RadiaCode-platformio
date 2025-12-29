# RadiaCode - ESP32 BLE Library for RadiaCode Radiation Detectors
# Copyright (c) 2025 Markus Geiger
# MIT License

cmake_minimum_required(VERSION 3.5)

if(ESP_PLATFORM)
	# Build RadiaCode as an ESP-IDF component
	
	# List of private requirements (dependencies)
	list(APPEND RADIACODE_PRIV_REQUIRES esp-nimble-cpp)
	
	# Check for Arduino framework as an ESP-IDF component
	# Arduino install using IDF component manager
	if(__COMPONENT_TARGETS MATCHES "___idf_espressif__arduino-esp32")
		list(APPEND RADIACODE_PRIV_REQUIRES arduino-esp32)
	# Manual installation of Arduino framework
	elseif(__COMPONENT_TARGETS MATCHES "__idf_arduino")
		list(APPEND RADIACODE_PRIV_REQUIRES arduino)
	# PlatformIO
	elseif(__COMPONENT_TARGETS MATCHES "___idf_framework-arduinoespressif32")
		list(APPEND RADIACODE_PRIV_REQUIRES framework-arduinoespressif32)
	endif()
	
	idf_component_register(
		SRCS
			"src/BluetoothTransport.cpp"
			"src/BytesBuffer.cpp"
			"src/Decoders.cpp"
			"src/RadiaCode.cpp"
			"src/RadiaCodeTypes.cpp"
		INCLUDE_DIRS
			"src"
		PRIV_REQUIRES
			${RADIACODE_PRIV_REQUIRES}
	)
	return()
endif()

# For non ESP-IDF builds (e.g., standard CMake projects)
project(RadiaCode VERSION 1.0.0)

# Library target for standard builds
add_library(RadiaCode
	src/BluetoothTransport.cpp
	src/BytesBuffer.cpp
	src/Decoders.cpp
	src/RadiaCode.cpp
	src/RadiaCodeTypes.cpp
)

target_include_directories(RadiaCode PUBLIC src)

# Set C++ standard
set_property(TARGET RadiaCode PROPERTY CXX_STANDARD 11)
set_property(TARGET RadiaCode PROPERTY CXX_STANDARD_REQUIRED ON)
