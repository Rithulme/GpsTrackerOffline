#include <WiFi.h>
#include <WebSocketsClient.h>
//For OTA libraries
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

//buttons and leds
const int toggleGPSButtonPin = 21;
const int sendButtonPin = 19;
const int clearDataButtonPin = 23;
const int startupLEDPin = 22;
const int positionAvailableLEDPin = 32;
const int recordingLEDPin = 33;

//modem data
const int PIN_DTR = 25;
const int PIN_TX = 27;
const int PIN_RX = 26;
const int PWR_PIN = 4;

// WebSocket client
WebSocketsClient webSocket;
const char* command ="";
bool commandOk = false;
String received="";

//LED setup
int blinkTime = 250;

int mainState = 0;
bool gpsPolling = false;
bool correctPosition = false;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 26, 27);

  //setting up wifi and websocket  
  delay(3000);
  setupWifiAndWebsocket();

  //start the OTA
  ArduinoOTA.setHostname("myesp32");
  ArduinoOTA.begin();

  //setup FileSystem
  setupFileSystem();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  IOSetup();
}

void loop() {
  webSocket.loop();
  ArduinoOTA.handle();
  ManageModemState();
}
