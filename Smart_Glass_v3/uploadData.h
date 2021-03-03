//https://esp-32-cam.000webhostapp.com/updatedata.php?Object1=Som&Value1=9.9&Object2=Sourav&Value2=89.9&Object3=Sommm&Value3=90.9&Object4=Soma&Value4=99.07&Object5=Sonu&Value5=99.9&Data=
String urlencode(String);
 
void uploadImg(camera_fb_t *fb){
  for (int i=0; i < 5; i++) {
    object[i] = urlencode(String(object[i]));
    //value[i] = urlencode(String(value[i]));
    Serial.print(object[i]);
    Serial.println(value[i]);
  }
  size_t size = fb->len;
  String buffer = base64::encode((uint8_t *) fb->buf, fb->len);

  buffer = urlencode(buffer);
  //String data = ("Object1=Som&Value1=9.9&Object2=Sourav&Value2=89.9&Object3=Sommm&Value3=90.9&Object4=Soma&Value4=99.07&Object5=Sonu&Value5=99.9&Data=" + buffer);
  String data = ("Object1="+object[0]+"&Value1="+String(value[0])+"&Object2="+object[1]+"&Value2="+String(value[1])+"&Object3="+object[2]+"&Value3="+String(value[2])+"&Object4="+object[3]+"&Value4="+String(value[3])+"&Object5="+object[4]+"&Value5="+String(value[4])+"&Data=" + buffer);
  //String data = ("Object1="+object[0]+"&Value1=1"+"Object2="+object[1]+"&Value2=2"+"Object3="+object[2]+"&Value3=3"+"Object4="+object[3]+"&Value4=4"+"Object5="+object[4]+"&Value5=5"+"&Data=" + buffer);
  //String data = ("Object1="+String(object[0])+"&Value1="+String(value[0])+"Object2="+String(object[1])+"&Value2="+String(value[1])+"Object3="+String(object[2])+"&Value3="+String(value[2])+"Object4="+String(object[3])+"&Value4="+String(value[3])+"Object5="+String(object[4])+"&Value5="+String(value[4])+"&Data=" + buffer);

  //String data = "{\"Object1\":\"Sourav\",\"Value1\":98.3,\"Object2\":\"Namrata\",\"Value2\":98.3,\"Object3\":\"Peter2\",\"Value3\":98.3,\"Object4\":\"Peter3\",\"Value4\":98.3,\"Object5\":\"Souraaavv\",\"Value5\":98.3,\"Data\":\"Hiiiiiii\"}";//":\"" + buffer + "\"}";
  
  Serial.println(data);
  
  /*WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }*/
  
 
  /*client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");*/

   /*client.println("POST / HTTP/1.1");
   client.print("Host: ");
   client.println(host);
   client.println("Accept: '*'/*");
   client.println("Content-Type: application/x-www-form-urlencoded");
   client.print("Content-Length: ");
   client.println(url.length());
   client.println();
   client.print(url);*/

    HTTPClient http;
    http.begin("https://esp-32-cam.000webhostapp.com/postdata.php");
    http.addHeader("Content-Type","application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(data);
    String payload = http.getString();

   delay(2000);//500
   Serial.println(payload);
   Serial.print("HTTP Response code: ");
   Serial.println(httpResponseCode);
   
  /*while(client.available()){
    String line = client.readStringUntil('\r\n\r\n');
    Serial.println(line);
  }*/

  Serial.println("Close connection");
  delay(3000);
  http.end();
}

String urlencode(String str){
  String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}
