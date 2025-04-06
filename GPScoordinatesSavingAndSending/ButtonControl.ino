void checkButton(){
  static bool previousState = false;
  bool state = digitalRead(toggleGPSButtonPin);

  if (state != previousState && mainState > 6) { // only allow toggle when startup completly done
    previousState = state;
    if (state) {
      gpsPolling = !gpsPolling;
      digitalWrite(recordingLEDPin, gpsPolling);
      delay(20);
    }
  }
}

void checkSendButton(){
  static bool previousState = false;
  bool state = digitalRead(sendButtonPin);

  if (state != previousState && mainState > 6) { // only allow toggle when startup completly done
    previousState = state;
    if (state) { // todo: check if connected
      Serial.println("Sending to node red");
      publishGpsData();
    }
  }
}

void checkDeleteButton(){
  static bool previousState = false;
  bool state = digitalRead(clearDataButtonPin);

  if (state != previousState && mainState > 6) { // only allow toggle when startup completly done
    previousState = state;
    if (state) { // todo: check if connected
      Serial.println("Clear data");
      clearData();
    }
  }
}