String readModem(){
  String msg="";
  bool lf = false;
  while (Serial2.available()) {
    byte byteUart = Serial2.read();
    printByteAsAscii(byteUart); //use this to visually print carriage-return and linefeed characters
    //Serial.print((char)byteUart);
    msg += (char)byteUart; //typecast byte to char
    if(byteUart == '\n'){
      //Serial.print("Received from modem: ");
      //Serial.println(msg);
      lf = true;
    }
  }
  if(lf){
    return msg;
  }
}

void printByteAsAscii(byte inputByte) { //used to display CarriageReturn and Linefeeds in terminal (if your terminal show this, turn it off in the terminal)
    if (inputByte == '\n') {
        Serial.println("<LF>");
    } else if (inputByte == '\r') {
        Serial.println("<CR>");
    } else {
        Serial.print((char)inputByte);
    }
}

bool sendModem(String ATcomm, String returnMsg){
  Serial.print("Sending AT command:");Serial.println(ATcomm);
  int retries=0;
  String receivedMsg = "";
  bool returnFound = false;
  Serial2.println(ATcomm);
  delay(1000);
  while(retries < 10 && receivedMsg.indexOf(returnMsg) == -1){
    receivedMsg = readModem();
    retries++;
    Serial.print("Try to receive expected return message:");Serial.print(returnMsg);Serial.print(" Attempt:");Serial.println(retries);
    delay(1000);
  }
  if(retries > 10) {
    Serial.print(returnMsg);Serial.print(" NOT found in return after sending command: ");Serial.print(ATcomm);Serial.println(" Sorry .... timed out / failed \n....RESET program, hang on for 10s");
    //reset the module so we won't get stuck in for example the send SMS terminal mode
    mainState = 0;
    return false;
  } else {
    Serial.print(returnMsg);Serial.print(" found in return after sending command: ");Serial.print(ATcomm),Serial.println(" succesful, all done!");
    return true;
  }
}

// void executeWebsocketCommand()
// {
//   if(command != ""){
//     Serial.println("Command received from WS or Serial:");
//     Serial.println(command);
//     Serial2.println(command);
//     answerViaWebsocket = true;
//     command = "";
//   }
// }