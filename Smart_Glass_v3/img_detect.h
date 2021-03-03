void classifyImage(camera_fb_t *fb) {

  size_t size = fb->len;
  String buffer = base64::encode((uint8_t *) fb->buf, fb->len);
  Serial.println(buffer);
  Serial.println();
  
  // http request to server
  String payload = "{\"inputs\": [{ \"data\": {\"image\": {\"base64\": \"" + buffer + "\"}}}]}";
  buffer = "";
    
  HTTPClient http;
  http.begin("https://api.clarifai.com/v2/models/" + model_id + "/outputs");
  http.addHeader("Content-Type", "application/json");     
  http.addHeader("Authorization", ("Key " + app_key)); //891e0781c55e4175920094840204a5dc"); 
  int response_code = http.POST(payload);
  
  if(response_code>0){
    Serial.println(response_code);
    //Serial.print("Returned String: ");
    String response = http.getString();
    Serial.println(response);
   
    // Classifying from the payload received
    const int jsonSize = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(20) + 3*JSON_OBJECT_SIZE(1) + 6*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 20*JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(6);
    DynamicJsonDocument doc(jsonSize);
    deserializeJson(doc, response);
    
    for (int i=0; i < 5; i++) {
      const char* name = doc["outputs"][0]["data"]["concepts"][i]["name"];
      const float p = doc["outputs"][0]["data"]["concepts"][i]["value"];

      object[i] = name;
      value[i] = p;
      Serial.println("=====================");
      Serial.print("Name:");
      Serial.println(name);
      Serial.print("Prob:");
      Serial.println(p);
      Serial.println();
    }
     
  }
  else {
    Serial.print("POST Error: ");
    Serial.print(response_code);
    Serial.println("  ERROR!!!");
    return;
  }
  delay(2000);
  http.end();
  delay(2000);
}
