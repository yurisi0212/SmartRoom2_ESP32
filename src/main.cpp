#include "IRremote.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>


#define IR_RECEIVE_PIN 12
#define IR_LED_PIN 14

const int light_ir_address = 0x459A;
const char ssid[] = "";
const char pass[] = "";
AsyncWebServer server(80);

StaticJsonDocument<512> doc;

uint8_t ir_status;

void setupNetwork(){

  if (!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(300);
    Serial.print(".");
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html"); });

  server.on(
      "/change_ir_status",
      HTTP_POST,
      [](AsyncWebServerRequest *request) {},
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
        String resjson = "";

        for (size_t i = 0; i < len; i++){
          resjson.concat(char(data[i]));
        }

        Serial.println(resjson);
        DeserializationError error = deserializeJson(doc, resjson);

        if (error){
          Serial.println("deserializeJson() faild");
          request->send(400);
        }else{
          ir_status = doc["IR_STATUS"];
          request->send(200);
        }
      });

  ir_status = 0;

  server.begin();

  Serial.println("Server start!");
}

void airOff(){
  uint16_t codes[99] = {3289, 1665, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410};
  IrSender.sendRaw(codes, sizeof(codes) / sizeof(codes[0]), NEC_KHZ);
}

void airOn(){
  uint16_t codes[243] = {3289, 1665, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 1242, 410, 1242, 410, 1242, 410, 1242, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 1242, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 1242, 410, 1242, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 438, 410, 1242, 410, 438, 410, 1242, 410, 1242, 410, 1242, 410, 438, 410, 1242, 410, 438, 410};
  IrSender.sendRaw(codes, sizeof(codes) / sizeof(codes[0]), NEC_KHZ);
}

void setup() {
  Serial.begin(115200);
  //IrReceiver.begin(IR_RECEIVE_PIN, true); 
  IrSender.begin(IR_LED_PIN);
  pinMode(IR_LED_PIN, OUTPUT);
  setupNetwork();
}
 
void loop() {
  
  switch (ir_status){
    case 1:
    IrSender.sendNEC(light_ir_address, 0x4, 0);
    break;
    case 2:
    IrSender.sendNEC(light_ir_address, 0x88, 0);
    break;
    case 3:
    IrSender.sendNEC(light_ir_address, 0x48, 0);
    break;
    case 4:
    IrSender.sendNEC(light_ir_address, 0xC4, 0);
    break;
    case 5:
    IrSender.sendNEC(light_ir_address, 0x44, 0);
    break;
    case 6:
    IrSender.sendNEC(light_ir_address, 0x21, 0);
    break;
    case 7:
    IrSender.sendNEC(light_ir_address, 0x22, 0);
    break;
    case 8:
    IrSender.sendNEC(light_ir_address, 0x23, 0);
    break;
    case 9:
      airOn();
    break;
    case 10:
     airOff();
    break;
    case 11:
      IrSender.sendNEC(light_ir_address, 0x23, 0);
      delay(100);
      IrSender.sendNEC(light_ir_address, 0x44, 0);
      delay(100);
      airOn();
    break;
    case 12:
      IrSender.sendNEC(light_ir_address, 0x4, 0);
      delay(100);
      airOff();
  }
  ir_status = 0;
  delay(500);
}