#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <FastLED.h>
#include <SPIFFS.h>

#define LED_PIN     4
#define NUM_LEDS    144
#define DATA_RATE   800000
#define RATE_LIMIT_MS 10 // Rate limit in milliseconds

CRGB leds[NUM_LEDS];
unsigned long lastProcessedTime = 0; // Timestamp of the last processed message

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup() {
 Serial.begin(115200);
 FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
 FastLED.setBrightness(100);

 // Initialize SPIFFS
 if (!SPIFFS.begin(true)) {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
 }

 // Connect to WiFi
 WiFi.begin("iliadbox-693C65", "4c6ckqh34tqdx253qhvw5t");
 while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
 }
 Serial.println("Connected to WiFi");

 // Web server setup
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
 });

 // WebSocket event handler
 ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                   void *arg, uint8_t *data, size_t len){
    if(type == WS_EVT_DATA){
      AwsFrameInfo *info = (AwsFrameInfo*)arg;
      if(info->final && info->index == 0 && info->len == len){
        // Check if enough time has passed since the last message was processed
        unsigned long currentTime = millis();
        if (currentTime - lastProcessedTime > RATE_LIMIT_MS) {
          lastProcessedTime = currentTime; // Update the last processed time
          // Handle WebSocket message
          if(len == 4) { // Assuming RGB color data
            uint8_t r = data[0];
            uint8_t g = data[1];
            uint8_t b = data[2];
            bool on = data[3];
            if(on) {
              fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
            } else {
              fill_solid(leds, NUM_LEDS, CRGB::Black);
            }
            FastLED.show();
          }
        }
      }
    }
 });

 server.addHandler(&ws);
 server.begin();
}

void loop() {
 // Nothing to do here
}
