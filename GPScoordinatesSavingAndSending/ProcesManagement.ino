void IOSetup(){
  pinMode(toggleGPSButtonPin, INPUT_PULLDOWN);
  pinMode(sendButtonPin, INPUT_PULLDOWN);
  pinMode(clearDataButtonPin, INPUT_PULLDOWN);
  pinMode(startupLEDPin, OUTPUT);
  pinMode(positionAvailableLEDPin, OUTPUT);
  pinMode(recordingLEDPin, OUTPUT);
}

void manageProcess(){
  static unsigned long lastPoll;

  executeWebsocketCommand();
  received = readModem();

  handleCommand(received);

  if (millis() - lastPoll > 15000) {
    Serial.println("polling GPS");
    GetGpsCoordinates();
    lastPoll = millis();
  }

  checkButton();
  checkSendButton();
  checkDeleteButton();  
}