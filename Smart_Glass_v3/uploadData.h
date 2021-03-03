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
  String data = ("Object1="+object[0]+"&Value1="+String(value[0])+"&Object2="+object[1]+"&Value2="+String(value[1])+"&Object3="+object[2]+"&Value3="+String(value[2])+"&Object4="+object[3]+"&Value4="+String(value[3])+"&Object5="+object[4]+"&Value5="+String(value[4])+"&Data=" + buffer);  
  Serial.println(data);
  

  HTTPClient http;
  http.begin("https://xyz.000webhostapp.com/postdata.php");
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(data);
  String payload = http.getString();

  delay(2000);//500
  Serial.println(payload);
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
   

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
