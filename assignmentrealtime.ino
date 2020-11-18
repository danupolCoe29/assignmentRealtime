#include <WiFi.h>

const char* ssid     = "Joker";
const char* password = "0869676747";
const char* host = "api.thingspeak.com";    // Host ของ thingspeak ไม่ต้องแก้ไข
const char* api   = "EWN6LXMSUVO09MKO";  
const int httpPort = 80;
#define analogPIN1 34
#define ledPIN 2
int val_light=0;
void light(void *pvParameter){

  pinMode(analogPIN1,OUTPUT);
  while(true) {
    val_light =map(analogRead(analogPIN1),0,4095,100,0);
    Serial.println(val_light);
    vTaskDelay(2000 / portTICK_RATE_MS);
  }
}
// ----------------------------------------------------------------------------------
void led_task(void *pvParameter){
  
  pinMode(ledPIN,OUTPUT);
  while(true) {
    if(val_light <30 )
      digitalWrite(ledPIN,HIGH);
    else
      digitalWrite(ledPIN,LOW);
    vTaskDelay(2000 / portTICK_RATE_MS);

  }
}
//-------------------------------------------------------------------------------------
void send_data(void *pvParameter){
  
  while(true){
    WiFiClient client;
      if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      }
    String url = "/update?api_key=";
    url += api;
    url += "&field1=";
    url += String(val_light);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    
    delay(10);
    vTaskDelay(2000 / portTICK_RATE_MS);
   }
 }
//---------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  xTaskCreate(&light, "light_task", 2048, NULL, 5, NULL);
  xTaskCreate(&led_task,"led_task",2048, NULL, 5, NULL);
  xTaskCreate(&send_data,"Send data to thinkspeak",2048,NULL,5,NULL);
  
  Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
  
}

void loop() {
  
}
