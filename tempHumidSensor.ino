#include <Wire.h>
#include <mlx90615.h>
#include <SPI.h>
#include <SSD_13XX.h>
#include "Adafruit_SHT31.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define __CS  12  //GPIO4 or GPIO2
#define __DC  15  //(D1)

SSD_13XX tft = SSD_13XX(__CS, __DC);

MLX90615 mlx = MLX90615();

Adafruit_SHT31 sht31 = Adafruit_SHT31();

const char *ssid = "WIFISSID";
const char *password = "WIFIPASSWORD";

int count = 0;

void setup() {
  mlx.begin();
  sht31.begin(0x44);
  tft.begin();

WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
  }
}

void loop() {
  float iratemp = mlx.get_ambient_temp();
  float irtemp = mlx.get_object_temp();
  float temp = sht31.readTemperature();
  float hum = sht31.readHumidity();

  if (count > 30) {
    count = 0;
    String text = String("http://api.thingspeak.com/update?api_key=APIKEYHERE&field1=") + iratemp + String("&field2=") + irtemp + String("&field3=") + temp + String("&field4=") + hum;
    HTTPClient http;
    http.begin(text);
    http.GET();
    http.end();
  } else {
    count++;
  }

  tft.clearScreen();
  tft.print("IR Ambient: ");
  tft.print(iratemp);
  tft.println(" C");
  tft.print("IR Object: ");
  tft.print(irtemp);
  tft.println(" C");
  tft.print("SHT Temp: ");
  tft.print(temp);
  tft.println(" C");
  tft.print("SHT Humi: ");
  tft.print(hum);
  tft.println(" %");
  delay(500);
}
