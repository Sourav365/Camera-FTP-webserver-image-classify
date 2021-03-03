
#include "settings.h"
#include "img_detect.h"
#include "save_to_sd.h"
#include "uploadData.h"
#include "img_capture.h"

// Tasks define
TaskHandle_t FtpTask; //CameraTask, AviWriterTask, 

void codeForFtpTask( void * parameter )
{
  uint32_t ulNotifiedValue;
  Serial.print("ftp, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  for (;;) {
    ftpSrv.handleFTP();
    delay(1);

  }
}

void setup() {
  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  
  pinMode(33,OUTPUT); //Built-in-LED
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,0);

  
  //Wi-Fi Connect
  WiFi.begin(ssid, password);
  int i=0;
  while (WiFi.status() != WL_CONNECTED) {
    i++;
    if(i==50)  ESP.restart(); //Restart after 5 Sec.
    delay(100); 
    digitalWrite(33,!digitalRead(33));
  }
  delay(5000);
  digitalWrite(33,1);
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  //Camera initiate
  setup_cam();
  setup_sd();
  ftpSrv.begin("esp", "esp");


  // Tasks
  xTaskCreatePinnedToCore(
      codeForFtpTask,
      "FtpTask",
      4096,       // heap
      NULL,
      4,          // prio higher than 1
      &FtpTask,
      0);         // on cpu 0

    delay(20);


  
}

void loop() {
  int detect=0;
  detect = Serial.parseInt();
  if(detect){
    capture_image();
    delay(10);
    detect=0;
  }
}
