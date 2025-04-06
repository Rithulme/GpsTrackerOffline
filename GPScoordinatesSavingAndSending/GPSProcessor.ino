String parseCGNSINF(String input) {
    // Remove the prefix "+CGNSINF: " to get the raw data
    int startIndex = input.indexOf(":") + 2;
    if (startIndex == -1) return "{}";  // Return empty JSON if not found

    input = input.substring(startIndex); // Extract only the data part

    // Create a JSON document
    JsonDocument coordinates = readData();
    int size = coordinates.size();
    
    // Array to store parsed values
    String values[20];  
    int index = 0;

    // Tokenize the input string
    int lastPos = 0;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ',' || i == input.length() - 1) {
            if (i == input.length() - 1) i++;  // Include last character
            values[index++] = input.substring(lastPos, i);
            lastPos = i + 1;
        }
    }

    // Assign values to JSON object
    coordinates[size]["Latitude"] = values[3].toFloat();
    coordinates[size]["Longitude"] = values[4].toFloat();
    coordinates[size]["Speed"] = values[6].toFloat();
    coordinates[size]["Course"] = values[7].toFloat();
    
    int fixStatus = values[8].toInt();
    int gnssUsed = values[15].toInt();
  
    checkSignalQuality(fixStatus, gnssUsed);
    
    // Serialize JSON to a string
    String jsonString;
    serializeJson(coordinates, jsonString);
    
    if(gpsPolling && correctPosition){
      
      //webSocket.sendTXT(jsonString);

      writeData(coordinates);
    }    

    return jsonString;
}

void checkSignalQuality(int available, int numberOfSatelites){
  if (available = 1 && numberOfSatelites >= 3)  {
    correctPosition = true;
    digitalWrite(positionAvailableLEDPin, true);
  }
  else {
    correctPosition = false;
    digitalWrite(positionAvailableLEDPin, false);
    Serial.println("Position unusable");
  }
}

void GetGpsCoordinates(){
  String GPSCommand = "AT+CGNSINF";
  Serial.println("Getting gps coordinates");
  Serial2.println(GPSCommand);
}