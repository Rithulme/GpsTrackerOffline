void modemPowerOn() {
  digitalWrite(PWR_PIN, HIGH);
  delay(1000);  //Datasheet Ton min = 1S
  digitalWrite(PWR_PIN, LOW);
  Serial.println("SIM7000 switched on!");
}

void modemPowerOff() {
  digitalWrite(PWR_PIN, HIGH);
  delay(1500);  //Datasheet Toff min = 1.2S
  digitalWrite(PWR_PIN, LOW);
  Serial.println("SIM7000 switched off!");
}

bool forceModem()
{
  for (int j = 0; j < 20; j++) {
    Serial2.print("AT\r\n");
    String input = Serial2.readString();
    if (input.indexOf("OK") >= 0) {
      Serial.println("Modem responded");
      return true;
    }
  }
    Serial.println("Modem is not online!!!");
    return false;
}

void ManageModemState(){
  static int previousState = -1;
  static unsigned long previousStep;
  bool commandOk = false;
  String message;
  switch(mainState){
    case 0:
      if (previousState != mainState) {
        webSocket.sendTXT("Starting SIM Init");
        previousState = mainState;
        previousStep = millis();
      }

      if(millis() - previousState > 7500){
        mainState = 1;
      }

      break;

    case 1:
      if (previousState != mainState) {
        pinMode(PWR_PIN, OUTPUT);
        digitalWrite(PWR_PIN, LOW);
        modemPowerOff();
        previousStep = millis();
        previousState = mainState;
      }

      if(millis() - previousState > 7500){        
        mainState = 2;
      }

      break;

    case 2:
      if (previousState != mainState) {
        modemPowerOn();
        previousStep = millis();
        previousState = mainState;
      }

      if(millis() - previousState > 7500){        
        mainState = 3;
      }

      break;

      case 3:
        previousState = mainState;
        if(forceModem()){
          mainState = 4;
        }
        else {
          Serial.println("Reset modem");
        }

        break;
      
      case 4:
        previousState = mainState;
        message = readModem();

        if(message.indexOf("PSUTTZ") != -1){
        mainState = 5;
        }

        break;
      
      case 5:
        //try sending command once, if it fails 'commandOk' is always false, after 1 second previous state is reset to trigger command again
        if (previousState != mainState) {
          previousState = mainState;
          previousStep = millis();
          commandOk = sendModemAndProcessResponse("AT+CGNSPWR=1","OK");
          Serial.println("at state 5");
        }       
        
        if(commandOk){
          mainState = 6;
        }
        else{
          if (millis() - previousStep > 1000) {
            previousState = 4;
          }
        }   

      case 6:
        //try sending command once, if it fails 'commandOk' is always false, after 1 second previous state is reset to trigger command again
        if (previousState != mainState) {
          previousState = mainState;
          previousStep = millis();
          commandOk = sendModemAndProcessResponse("AT+SGPIO=0,4,1,1","OK");
          Serial.println("at state 6");
        }       
        
        if(commandOk){
          mainState = 7;
        }
        else{
          if (millis() - previousStep > 1000) {
            previousState = 5;
          }
        }

      case 7:
        digitalWrite(startupLEDPin, true);
        manageProcess();

  }
}