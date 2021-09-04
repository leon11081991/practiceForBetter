// Flag's Block 產生的草稿碼

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "data/webpages.h"
#include <Servo.h>
#include <FlagIFTTT.h>
#include "my.h"  //dynamicTasking 1/5
#include <ESP8266mDNS.h> //mDNS 1/3
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

int pinNo;
String ledId;
int vrValue;
boolean btValue;
String roValue;
String ledSw;
int ledNo;
ESP8266WebServer _esp8266WebServer(80);
Servo motor;

bool _softAPString(String ssidStr) {
  char ssidChr[64];
  ssidStr.toCharArray(ssidChr, 63);
  return WiFi.softAP(ssidChr, u8"", 1,false);
}

void handleRoot() {
  if (!_esp8266WebServer.authenticate("user", "123456")) {  //authtoken 1/3
      return _esp8266WebServer.requestAuthentication();     //authtoken 2/3
    }                                                       //authtoken 3/3
#ifndef WEBPAGE_IN_PROGMEM
  _esp8266WebServer.send(200, "text/html", mainPage);
#else
  _esp8266WebServer.send_P(200, PSTR("text/html"), mainPage);
#endif
}

void handleNotFound() {
#ifndef WEBPAGE_IN_PROGMEM
  _esp8266WebServer.send(404, "text/html", errorPage);
#else
  _esp8266WebServer.send_P(404, PSTR("text/html"), errorPage);
#endif
}

void handleSetting() {
#ifndef WEBPAGE_IN_PROGMEM
  _esp8266WebServer.send(200, "text/html", settingPage);
#else
  _esp8266WebServer.send_P(200, PSTR("text/html"), settingPage);
#endif
}

void led() {
  if (_esp8266WebServer.hasArg(u8"id") && _esp8266WebServer.hasArg(u8"sw")) {
    ledId = _esp8266WebServer.arg(u8"id");
    ledSw = _esp8266WebServer.arg(u8"sw");
    ledNo = ledId.toInt();
    Serial.println((String(u8"請求參數:") + String(ledId) + String(u8"&") + String(ledSw)));
    if (ledNo == 16 || (ledNo == 5 || (ledNo == 4 || ledNo == 0))) {
      if (ledSw == u8"on" || (ledSw == u8"off" || (ledSw == u8"toggle" || ledSw == u8"xblink"))) {
        if (ledSw == u8"on") {
          pinMode(ledNo, OUTPUT);
          digitalWrite(ledNo, HIGH);
        } else if (ledSw == u8"off") {
          pinMode(ledNo, OUTPUT);
          digitalWrite(ledNo, LOW);
        } else if (ledSw == u8"toggle") {
          pinMode(ledNo, OUTPUT);
          digitalWrite(ledNo, (!digitalRead(ledNo)));
        } else if (ledSw == u8"blink") {
          oLoopManager.add(ledId, &ledBlink, 500); //dynamicTasking 3/5
        } else if (ledSw == u8"xblink") {
          oLoopManager.remove(ledId); //dynamicTasking 4/5
        }
        _esp8266WebServer.send(200, u8"text/plain", String((String(u8"OK") + String(ledNo) + String(u8":") + String(digitalRead(ledNo) ? u8"on" : u8"off"))));
      } else {
        _esp8266WebServer.send(400, u8"text/plain;charset=utf-8", String(u8"id/sw動作錯誤?"));
      }
    } else {
      _esp8266WebServer.send(400, u8"text/plain;charset=utf-8", String(u8"id/sw腳位錯誤?"));
    }
  } else {
    _esp8266WebServer.send(400, u8"text/plain;charset=utf-8", String(u8"id/sw參數錯誤?"));
  }
}

void sensor() {
  if (_esp8266WebServer.arg(u8"id") == u8"vr") {
    vrValue = analogRead(A0);
    _esp8266WebServer.send(200, u8"text/plain;charset=utf-8", String(vrValue));
    if (vrValue > 800) {
      Serial.println((fireIFTTTEvent(u8"LINEnotify", u8"bzFIVZS7LlvVz_AS-j4Sgm", String(u8"VR"), String(u8"超過臨界值"), String((String)(vrValue))) == 200));
      Serial.println((fireIFTTTEvent(u8"GoogleSheets", u8"bzFIVZS7LlvVz_AS-j4Sgm", String((String)(vrValue)), String(u8""), String(u8"")) == 200));
    }
  } else if (_esp8266WebServer.arg(u8"id") == u8"bt") {
    btValue = digitalRead(D6);
    _esp8266WebServer.send(200, u8"text/plain;charset=utf-8", String(btValue));
  } else {
    _esp8266WebServer.send(400, u8"text/plain;charset=utf-8", String(u8"id/參數錯誤?"));
  }
}

void servo() {
  if (_esp8266WebServer.hasArg(u8"ro")) {
    roValue = _esp8266WebServer.arg(u8"ro");
    if (roValue.toInt() >= 0 && roValue.toInt() <= 180) {
      motor.write(roValue.toInt());
      _esp8266WebServer.send(200, u8"text/plain;charset=utf-8", String((String(u8"Motor:") + String(roValue))));
    } else {
      _esp8266WebServer.send(400, u8"text/plain;charset=utf-8", String(u8"ro參數錯誤?"));
    }
  } else {
    _esp8266WebServer.send(400, u8"text/plain;charset=utf-8", String(u8"ro參數錯誤?"));
  }
}
//dynamicTasking 2/5
void ledBlink(int pinNo) {
  pinMode(pinNo, OUTPUT);
  digitalWrite(pinNo, (!digitalRead(pinNo)));
}


// setup() 會先被執行且只會執行一次
void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, INPUT_PULLUP);
  motor.attach(D4);
  pinMode(A0, INPUT);

  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D5, LOW);
  Serial.println(u8"開始連線:");
  WiFi.begin(u8"CHOU", u8"24323890");
  while (!(WiFi.status() == WL_CONNECTED)) {
    delay(1000);
    Serial.print(u8".");
  }
  Serial.print((String(u8"IP:") + String((WiFi.localIP().toString()))));
  WiFi.mode(WIFI_AP_STA);
  while (!_softAPString((String(u8"ESP8266:") + String((WiFi.localIP().toString())))));
  MDNS.begin("ESP826605");  //mDNS 2/3 http:
  _esp8266WebServer.on("/", handleRoot);
  _esp8266WebServer.onNotFound(handleNotFound);
  _esp8266WebServer.on("/setting", handleSetting);
  _esp8266WebServer.begin();
  _esp8266WebServer.on("/led", led);
  _esp8266WebServer.on("/sensor", sensor);
  motor.write(0);
  _esp8266WebServer.on("/servo", servo);
  oLoopManager.add("12",&button_detect,100);///SR04 1/3
  digitalWrite(D5, HIGH);
  delay(1000);
  digitalWrite(D5, LOW);
  delay(1000);

}

// loop() 裡面的程式會不斷重複執行
///SR04 2/3
byte eventState=2; 
byte sumTimes=0;
int sum=0,average=0;
void loop() {
  _esp8266WebServer.handleClient();
  oLoopManager.update();  //dynamicTasking 5/5
  MDNS.update(); //mDNS 3/3

  /// SR04 3/3
  if(button_counter%2==1){
    int distance=sr04(13,15);
    if(distance>0){    
      sum+=distance;
      sumTimes++;
    }
    if(sumTimes==10){
      average=sum/10;
      sum=0;
      sumTimes=0;
      Serial.println("SR04:"+String(average));
      if(average>0 && average<=10  && eventState!=0){
        eventState=0;
        oLoopManager.add("14",&ledBlink,100);
        oLoopManager.nodes["14"].interval=100;
        Serial.println((fireIFTTTEvent(u8"LINEnotify", u8"bzFIVZS7LlvVz_AS-j4Sgm", String("SR04"), String(u8"有人闖入"), String(u8"")) == 200));
      }
      else if(average>10 && average<=30 && eventState!=1){
        eventState=1; 
        oLoopManager.add("14",&ledBlink,500);
        oLoopManager.nodes["14"].interval=500;
        Serial.println((fireIFTTTEvent(u8"LINEnotify", u8"bzFIVZS7LlvVz_AS-j4Sgm", String("SR04"), String(u8"有人靠近"), String(u8"")) == 200));
      }
      else if(average>30 && eventState!=2){ 
        eventState=2; 
        oLoopManager.remove("14");
        Serial.println((fireIFTTTEvent(u8"LINEnotify", u8"bzFIVZS7LlvVz_AS-j4Sgm", String("SR04"), String(u8"無人靠近"), String(u8"")) == 200));
      } 
    }
  }else{oLoopManager.remove("14");}
}
