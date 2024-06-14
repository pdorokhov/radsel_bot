/*#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>*/

#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASS "WIFI_PASS"
#define BOT_TOKEN "BOT_TOKEN"
#include <FastBot.h>
FastBot bot(BOT_TOKEN);

/*const int iStatus = D1;*/
const int iArm  = D2;
const int iMove = D3;
const int iFire = D4;
bool xStatus = false;
bool xFireOn = false;
bool motionDetected = false;

/*IPAddress ip(192, 168, 88, 203);
IPAddress gateway(192, 168, 88, 1);
IPAddress subnet(255, 255, 255, 0);*/


/*-----------------------------INTERRUPTS-------------------------------*/
void ICACHE_RAM_ATTR detectsMovement() { 
  if (xStatus == true) {motionDetected = true;}}
void ICACHE_RAM_ATTR detectsFire() { xFireOn = true; }

/*-------------------------------SETUP---------------------------------*/
void setup() 
{
  /*pinMode(iStatus, INPUT);*/
  pinMode(iArm, OUTPUT);
  pinMode(iMove, INPUT);
  pinMode(iFire, INPUT);
  digitalWrite(iArm, true);
  connectWiFi();
  bot.attach(newMsg);
  attachInterrupt(digitalPinToInterrupt(iMove), detectsMovement, FALLING);
  attachInterrupt(digitalPinToInterrupt(iFire), detectsFire, FALLING);
  
  bot.sendMessage("\nСоединение с сетью WIFI установлено", "CHAT_ID");
  bot.sendMessage("IP-адрес: 192.168.88.203", "CHAT_ID");
  if (xStatus){ bot.sendMessage("Режим: ОХРАНА", "CHAT_ID");}
    else{ bot.sendMessage("Режим: НАБЛЮДЕНИЕ", "CHAT_ID"); }
}

/*----------------------------FUN_FASTBOT-----------------------------*/
void newMsg(FB_msg& msg) 
{ 
  if (msg.text == "/arm") 
  { digitalWrite(iArm, false); xStatus = true;}
  
  else if (xStatus == true and msg.text == "/disarm") 
  { xStatus = false; digitalWrite(iArm, true);}
 
  else if (msg.text == "/status") 
  { if (xStatus){ bot.sendMessage("Режим: ОХРАНА", msg.chatID);}
    else{
      bot.sendMessage("Режим: НАБЛЮДЕНИЕ", msg.chatID);
    }
    if (motionDetected){ bot.sendMessage("Датчик движения: АКТИВЕН", msg.chatID);}
    else{
      bot.sendMessage("Датчик движения: НЕ активен", msg.chatID);
    }
  if (xFireOn){ bot.sendMessage("Датчик задымления: АКТИВЕН", msg.chatID);}
    else{
      bot.sendMessage("Датчик задымления: НЕ активен", msg.chatID);
    }
  }
}

/*---------------------------LOOP----------------------------------*/
void loop() {

bot.tick();
  
if (motionDetected == true){ bot.sendMessage("Движение!", "CHAT_ID"); motionDetected = false; }
if (xFireOn == true){ bot.sendMessage("Задымление!", "CHAT_ID"); xFireOn = false; }
}
void connectWiFi() {
  delay(2000);
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  //WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
  Serial.println(WiFi.localIP());
}
