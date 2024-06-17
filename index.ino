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
const int iMove1 = D3;
const int iMove2 = D4;
bool xStatus = false;
bool motionDetected2 = false;
bool motionDetected1 = false;

/*IPAddress ip(192, 168, 88, 203);
IPAddress gateway(192, 168, 88, 1);
IPAddress subnet(255, 255, 255, 0);*/


/*-----------------------------INTERRUPTS-------------------------------*/
void ICACHE_RAM_ATTR detectsMovement1() { 
  if (xStatus == true) {motionDetected1 = true;}}
void ICACHE_RAM_ATTR detectsMovement2() 
  if (xStatus == true) {motionDetected2 = true;}}
/*-------------------------------SETUP---------------------------------*/
void setup() 
{
  /*pinMode(iStatus, INPUT);*/
  pinMode(iArm, OUTPUT);
  pinMode(iMove1, INPUT);
  pinMode(iMove2, INPUT);
  digitalWrite(iArm, true);
  connectWiFi();
  bot.attach(newMsg);
  attachInterrupt(digitalPinToInterrupt(iMove1), detectsMovement1, FALLING);
  attachInterrupt(digitalPinToInterrupt(iMove2), detectsMovement2, FALLING);
  
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
    if (motionDetected1){ bot.sendMessage("Датчик движения комната: АКТИВЕН", msg.chatID);}
    else{
      bot.sendMessage("Датчик движения комната: НЕ активен", msg.chatID);
    }
  if (motionDetected2){ bot.sendMessage("Датчик движения коридор: АКТИВЕН", msg.chatID);}
    else{
      bot.sendMessage("Датчик движения коридор: НЕ активен", msg.chatID);
    }
  }
}

/*---------------------------LOOP----------------------------------*/
void loop() {

bot.tick();
  
if (motionDetected1 == true){ bot.sendMessage("Движение комната!", "CHAT_ID"); motionDetected1 = false; }
if (motionDetected2 == true){ bot.sendMessage("Движение коридор!", "CHAT_ID"); motionDetected2 = false; }
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
