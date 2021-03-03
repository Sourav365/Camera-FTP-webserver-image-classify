int pictureNumber = 0;

void setup_sd(){
  Serial.println("Starting SD Card");
  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
    return;
  }
  
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }
  EEPROM.begin(EEPROM_SIZE);
}

void save_to_sd(camera_fb_t *fb){
  int tot = SD_MMC.totalBytes() / (1024 * 1024);
  int use = SD_MMC.usedBytes() / (1024 * 1024);
  
  pictureNumber = EEPROM.read(0) + 1;
  String path = sd_path + "/picture " + String(pictureNumber) +".jpg";
  fs::FS &fs = SD_MMC; 
    
  File file = fs.open(path.c_str(), FILE_WRITE);
  
  if(!file){
    Serial.println("Failed to open file in writing mode");
  } 
  else {   //line 714
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved file to path: %s\n", path.c_str());
    EEPROM.write(0, pictureNumber);
    EEPROM.commit();
  }
  file.close();
}
