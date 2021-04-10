#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <painlessMesh.h>

#include "credentials.hpp"

// https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html#compression
#define ATOMIC_FS_UPDATE


#define DEBUG true // flag to turn on/off debugging
#define Serial if(DEBUG)Serial


// #define BLYNK_PRINT        Serial
// #define B_TEMRMINAL        V0


static void init_wifi(void);
static void init_ota(void);
static void init_blynk(void);

// static WidgetTerminal blynk_terminal(B_TEMRMINAL);


void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  init_wifi();
  init_ota();
  init_blynk();
}


void loop() {
  ArduinoOTA.handle();
  Blynk.run();
}


static void init_wifi(void) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);        // Connect to the network
  Serial.print("\nWI-FI Connecting");

  while (WiFi.status() != WL_CONNECTED) {  // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
  }

  Serial.println("\nConnection established");
  Serial.print("IP:\t");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI:\t");
  Serial.println(WiFi.RSSI());
}


static void init_ota(void) {
  Serial.println("\nOTA init");
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });

  ArduinoOTA.begin();
  Serial.printf("OTA Ready, free space available: %.f bytes\n",(float)ESP.getFreeSketchSpace());
}


static void init_blynk(void) {
  Blynk.begin(BLYNK_TOKEN, WIFI_SSID, WIFI_PASS);
  Serial.println("\nBlynk init successful");
  // blynk_terminal.println(F("Init successful"));
  // blynk_terminal.flush();
}
