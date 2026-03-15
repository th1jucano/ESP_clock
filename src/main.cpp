#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wifi.h>
#include <time.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

const char* ssid = "SPRULES_2G";
const char* password = "MorganArthur2610";
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 3600;
const int daylightOffset_sec = 0;
bool showDots = true;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
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
  display.setTextSize(3);
  display.setCursor(20, 20);
  display.printf("%02d%c%02d", timeinfo.tm_hour, showDots ? ':' : ' ', timeinfo.tm_min);
  showDots = !showDots;  // inverte: true vira false, false vira true
  desenharBarrasWiFi(110, 2);
  display.display();
  delay(1000);

}

