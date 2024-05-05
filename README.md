# Simple WS2812 RGB LED strip controller using an ESP32

## Description
This code creates a web server and WebSocket server that allows clients to connect and control a strip of WS2812 LEDs by sending RGB color data and an on/off state over the WebSocket connection. The server serves a web page from SPIFFS, which could be used to interact with the LEDs through a web interface allowing for both color selection and on/off toggling with real-time feedback to the server.

### Libraries Used
#### In main.cpp 
- **Arduino.h**: Standard Arduino library for basic functions.
- **ESPAsyncWebServer.h**: Library for creating an asynchronous web server on ESP32/ESP8266.
- **AsyncTCP.h**: Library for asynchronous TCP connections, required by ESPAsyncWebServer.
- **FastLED.h**: Library for controlling WS2812 LEDs.
- **SPIFFS.h**: Library for accessing the SPI Flash File System (SPIFFS) on ESP32/ESP8266.
#### In index.html
- **pikr**: JavaScript Color-Picker library.

### Constants and Global Variables to update
- **LED_PIN**: The GPIO pin connected to the data input of the LED strip.
- **NUM_LEDS**: The number of LEDs in the strip.
- **RATE_LIMIT_MS**: The minimum time between processing WebSocket messages to prevent flooding.
- **ssid**: Holds the SSID of the network to connect to.
- **password**: Holds the password of the network to connect to.

### Setup Function
- Initializes the serial communication at 115200 baud for debugging.
- Sets up the LED strip with FastLED.addLeds(), specifying the LED type, pin, and color order.
- Sets the brightness of the LEDs.
- Initializes SPIFFS and checks for errors.
- Connects to a WiFi network with a specified SSID and password.
- Sets up the web server to serve an index.html file from SPIFFS when the root URL ("/") is accessed.
- Sets up a WebSocket event handler to process incoming messages.
- Checks if the message is a final frame and if its length matches the expected data size.
- Applies a rate limit to prevent processing messages too frequently.
- Interprets the message as RGB color data and a boolean indicating whether the LEDs should be turned on or off.
- Updates the LED strip accordingly and displays the changes.
- Adds the WebSocket handler to the server and starts the server.
