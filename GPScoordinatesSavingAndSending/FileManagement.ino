void setupFileSystem(){
  // Initialize LittleFS with error handling
  Serial.println("Initializing LittleFS...");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed. Attempting to format...");
    if (LittleFS.format()) {
      Serial.println("LittleFS formatted successfully.");
      if (!LittleFS.begin()) {
        Serial.println("Failed to mount LittleFS after formatting.");
        while (true); // Halt execution
      }
    } else {
      Serial.println("LittleFS format failed.");
      while (true); // Halt execution
    }
  } else {
    Serial.println("LittleFS mounted successfully.");
  } 
}

JsonDocument readData(){
  JsonDocument jsonData;

  File dataFile = LittleFS.open("/coordinates.txt", FILE_READ);
  if(!dataFile || dataFile.size() <= 0){
    dataFile = LittleFS.open("/coordinates.txt", FILE_WRITE);
    dataFile.print("[]");
    dataFile.close();
    Serial.println("created new file");
    return jsonData;
  }
  String fileContents = String(dataFile.readString());
  deserializeJson(jsonData, fileContents);

  //Serial.println("jsondata: " + fileContents);

  return jsonData;
}

String readDataAsString(){
  File dataFile = LittleFS.open("/coordinates.txt", FILE_READ);
  if(!dataFile || dataFile.size() <= 0){
    dataFile = LittleFS.open("/coordinates.txt", FILE_WRITE);
    dataFile.print("[]");
    dataFile.close();
    Serial.println("created new file");
    return "[]";
  }

  String fileContents = String(dataFile.readString());

  return fileContents;
}

void writeData(JsonDocument jsonData){
  File dataFile = LittleFS.open("/coordinates.txt", FILE_WRITE);

  String serializedData;

  serializeJson(jsonData, serializedData);
  //Serial.println("writing file content" + serializedData);
  dataFile.print(serializedData);
  dataFile.close();
}

void clearData(){
  Serial.println("deleting data");
  LittleFS.remove("/coordinates.txt");
  File dataFile = LittleFS.open("/coordinates.txt", FILE_WRITE);
  dataFile.print("[]");
  dataFile.close();
}