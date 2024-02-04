#include <SimpleDHT.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
int pinDHT11 = 8;
int capteur_lum = 0; 
float analog_lum;
float photocell;
SimpleDHT11 dht11;
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);   
  display.clearDisplay();  
  display.display();
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
}
void loop() {
  byte temperature = 0;
  byte humidity = 0;
 int err = SimpleDHTErrSuccess;
 analog_lum = analogRead(capteur_lum);
 photocell = analog_lum /3.5; 
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) 
  {
    Serial.print("Read DHT11 failed, err="); 
    Serial.println(err);
    delay(1000);
    return;
  }
  display.clearDisplay();  
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(5,9);  
  display.println("temperature:");
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(85,9);
  display.println(bmp.readTemperature()); 
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(110,9);  
  display.println(" 'C ");
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(5,19);  
  display.println("humidite:");
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(85,19);  
  display.println(humidity); 
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(110,19);  
  display.println(" % ");   
  display.display();  
  display.clearDisplay();
delay(1500);
  display.clearDisplay();
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(5,9);  
  display.println("Pressure:");
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(65,9);  
  display.print(bmp.readAltitude(101500));
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(110,9);  
  display.println(" Pa ");
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(5,19);  
  display.println("Altitude:"); 
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(65,19); 
  display.print(bmp.readAltitude());
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(110,19);  
  display.println(" m ");
  display.display();  
  display.clearDisplay();
  delay(1500);
  display.clearDisplay();  
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(5,9);  
  display.println("luminosite:");
  display.setTextSize(1);  
  
  display.setTextColor(WHITE);  
  display.setCursor(75,9);  
  display.println(photocell);
  display.setTextSize(1);  
  display.setTextColor(WHITE);  
  display.setCursor(110,9);  
  display.println(" % ");
  display.display();  
  display.clearDisplay();
  delay(1500);
  }
   
