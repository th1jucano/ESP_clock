#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <time.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 3600;
const int daylightOffset_sec = 0;
bool showDots = true;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiMulti wifiMulti;

void desenharBarrasWiFi(int x, int y) {
  int rssi = WiFi.RSSI();
  int barras = 0;

  if (rssi > -50) barras = 4;
  else if (rssi> -60) barras = 3;
  else if (rssi> -70) barras = 2;
  else if (rssi> -80) barras = 1;

  // Desenhar triângulos
  for (int i = 0; i < 4; i++) { 
    int alturaMax = 3 + (i * 3);
    int yBarra = y + (12 - alturaMax);

    if (i < barras) {
      display.fillRect(x + (i * 4), yBarra, 3, alturaMax, SSD1306_WHITE);
    } else { 
      display.drawRect(x + (i * 4), yBarra, 3, alturaMax, SSD1306_WHITE);

    }
  }
}

void setup() {
  Serial.begin(115200);

  wifiMulti.addAP("SPRULES_2G", "MorganArthur2610");
  wifiMulti.addAP("2ANDAR", "MorganArthur2610");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("Falha nesse caralho");
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Display ok");
  display.display();
  delay(2000);
  
  //Wifi

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Conectando Wifi...");
  display.display();

  // WiFi.begin(ssid, password);
  if (wifiMulti.run() == WL_CONNECTED){
    Serial.println("Conectado na rede: " + WiFi.SSID());
  }

  while (wifiMulti.run() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("Conectado!");
  display.display();
  delay(2000);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void loop() {
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Falha ao obter hora");
    return;
  }
  
  display.clearDisplay();
  display.setCursor(0,4);
  display.setTextSize(1);
  display.print(WiFi.SSID());
  display.setTextSize(3);
  display.setCursor(20, 20);
  display.printf("%02d%c%02d", timeinfo.tm_hour, showDots ? ':' : ' ', timeinfo.tm_min);
  showDots = !showDots;  // inverte: true vira false, false vira true
  display.setTextSize(1);
  display.setCursor(34,55);
  display.printf("%02d/%02d/%04d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
  desenharBarrasWiFi(110, 2);
  display.display();
  delay(1000);

}

