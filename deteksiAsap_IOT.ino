#define BLYNK_TEMPLATE_ID "TMPL6J3xrSKJ3"
#define BLYNK_TEMPLATE_NAME "SmokeDetector"
#define BLYNK_AUTH_TOKEN "ud2ihruRAbMiKSZg7wMs2B1ZlI-0U6Tr"
#define BLYNK_PRINT Serial
#define APP_DEBUG
#define USE_NODE_MCU_BOARD

#include <SimpleWiFiClient.h>
#include <ArduinoJson.h>
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <MQ2.h>
#include <SPI.h>
#include <Wire.h>
#include <CTBot.h>

String token = "6211043346:AAEtAq5GxTfACWFlO1JjKGdfQBp8XEF7RrE";
const int bot_id = 1651597617;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Royraflmp";
char pass[] = "royrafles";

#define buzzer  D6 
#define ledHijau D5
#define ledMerah D8

CTBot mybot;
BlynkTimer timer;

int nada;
int pin = A0;
float smoke;
MQ2 mq2(pin);



void setup(){
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(buzzer, OUTPUT);
  pinMode(ledMerah, OUTPUT);
  pinMode(ledHijau, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(ledHijau, HIGH);
  digitalWrite(ledMerah, LOW);
  mq2.begin();
  sendSensorData();
  timer.setInterval(2000L, kirim_blynk);

  Serial.println("Memulai Telegram Bot");
  mybot.wifiConnect(ssid, pass);
  mybot.setTelegramToken(token);

  if(mybot.testConnection()) {
    Serial.println("Koneksi Berhasil");
  }
  else {
    Serial.println("Koneksi Gagal");
  }
  
}

void loop() {
  sendSensorData();
  pesan();
  Blynk.run();
  timer.run();
 
}

BLYNK_WRITE(V4) {
  nada = param.asInt();
}

void sendSensorData(){
  float* values = mq2.read(true); 
  smoke = mq2.readSmoke();
  Blynk.virtualWrite(V0, ledHijau);
  Blynk.virtualWrite(V1, ledMerah);
  Blynk.virtualWrite(V2, buzzer);
  Blynk.virtualWrite(V3, smoke);
  Blynk.virtualWrite(V4, nada);
  
  if (smoke > 1 ) {
    
    Blynk.logEvent("smoke", "Bahaya, Ada Asap!!!!!");
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledMerah, HIGH);
    digitalWrite(ledHijau, LOW);
    tone(buzzer, nada);
  }
  else {
    digitalWrite(buzzer, LOW);
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledHijau, HIGH);
    noTone(buzzer); 
  }

}

void kirim_blynk(){
  
  Blynk.virtualWrite(V3, smoke);
  if(smoke > 1) {
Blynk.virtualWrite(V0, 1);
    Blynk.virtualWrite(V1, 0);
  }
  else  {
    
        Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, 1);
  }
  Blynk.virtualWrite(V4, nada);
}

void pesan(){
  TBMessage msg;

  if(smoke > 1) {
    mybot.sendMessage(bot_id, "Notifikasi : ADA ASAP !");
  
  }
}



