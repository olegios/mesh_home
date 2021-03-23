#include <Arduino.h>
#include <WiFi.h>

// #include "painlessMesh.h"
#include "credentials.hpp"


static void init_wifi(void);


void setup() {
  Serial.begin(115200);
  init_wifi();
}


void loop() {
  Serial.println(WiFi.status());
  delay(500);
}


static void init_wifi(void) {
  Serial.begin(115200);
  delay(500);
  Serial.println('\n');

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);        // Connect to the network
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {  // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.println("Connection established");
  Serial.print("IP:\t");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI:\t");
  Serial.println(WiFi.RSSI());
}
