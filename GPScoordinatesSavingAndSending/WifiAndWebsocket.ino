void setupWifiAndWebsocket(){
  const char* websocket_server = "192.168.0.182";
  const char* websocket_path = "/ws/example";
  const int websocket_port = 1880;  

  connectToWiFi();
  webSocket.begin(websocket_server, websocket_port, websocket_path);
  webSocket.onEvent(onWebSocketEvent); //callback
}

void connectToWiFi() {
  const String ssid = "thosar";
  const String password = "BogVer88";

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void onWebSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket Disconnected!");
      break;
    case WStype_CONNECTED:
      Serial.println("WebSocket Connected!");
      break;
    case WStype_TEXT:
      Serial.printf("WebSocket Message: %s\n", payload);

      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, payload);
      // Extract command value
      String command = String(doc["payload"]["command"]);

      Serial.println("command is: " + String(command));

      handleWebsocketCommand(command);

      break;
  }
}

void handleWebsocketCommand(String command){
  if (command == "U") {
    publishGpsData();
  }
  else if (command == "D") {
    clearData();
  }
  else {
    Serial.println("Unknown command");
  }
}

void publishGpsData(){
  String message = readDataAsString();

  Serial.println("memoryString: " + message);

  message = "{\"Dataset\":" + message + "}";

  Serial.println("sending " + message); 
  
  webSocket.sendTXT(message);
}