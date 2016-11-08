#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define STRAP_URL = "http://snip/bootstrap.php";
#define RECV_URL  = "http://snip/droneinput.php";
#define OTAU_URL  = "http://snip/update_fw_ota.php";

void setup() {

Serial.begin(115200);                               
WiFi.begin("<snip>","move_in_silence");  // update to use multi

  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);
  Serial.println("Waiting for connection");
  }
}

void loop() {

if(WiFi.status()== WL_CONNECTED){
  HTTPClient http;
  http.begin("http://snip/bootstrap.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  int httpCode = http.POST("chip_id=" + String(ESP.getChipId()));
  
  if(httpCode == int("202")) {
    Serial.println("chip_id already known to server\n");
    //String payload = http.getString();
    Serial.println(httpCode);   //Print HTTP return code
    http.end();  //Close connection
  } else {
    Serial.println("Error");
    Serial.println(httpCode);
  }
  delay(30000);  //Send a request every 30 seconds
}
}
