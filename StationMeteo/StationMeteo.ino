/*
 * Ce bloc d'inclusions importe les bibliothèques nécessaires pour utiliser les fonctionnalités de l'ESP32 ou ESP8266,
 * la communication Wi-Fi,
 * la communication avec Firebase,
 * la gestion des capteurs DHT (température et humidité)
 * et le capteur BMP085 (pression atmosphérique).
*/
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>

/*
 * Ces lignes définissent les constantes pour le réseau Wi-Fi (SSID et mot de passe),
 * la clé API Firebase,
 * l'URL de la base de données Firebase,
 * les broches utilisées pour les capteurs et les LED.
*/
#define WIFI_SSID "Freebox-70F401"
#define WIFI_PASSWORD "perimunt#5-officiarii-sumentis9-desiliunt"
#define API_KEY "AIzaSyBSN6L9qMQ1S1t9S9DLbG5pu0KMYr2s1qE"
#define DATABASE_URL "https://databaseesp8266capture-default-rtdb.europe-west1.firebasedatabase.app" 
#define DHTPIN 14
#define LEDOK 13
#define LEDWARN 15
#define CAT_LUM A0
#define SCREEN_ADDRESS 0x3C

/*
 * Ces lignes déclarent des objets nécessaires pour communiquer avec Firebase,
 * gérer l'authentification,
 * configurer Firebase
 * et interagir avec les capteurs DHT et BMP085.
*/
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
DHT dht(DHTPIN, DHT11);
Adafruit_BMP085 bmp;

/*
 * Variables de contrôle pour la connexion à Firebase et la gestion du temps.
*/
bool signupOK = false;
unsigned long previousMillis = 0;

/*
 * setup qui configure les broches,
 * la communication série,
 * les capteurs
 * le réseau Wi-Fi,
 * et configure Firebase pour l'envoi de données.
*/
void setup(){  
    

  // Initialisation des broches
  pinMode(CAT_LUM, INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(LEDOK, OUTPUT);
  pinMode(LEDWARN, OUTPUT);

  // Initialisation de la communication série
  // Initialisation des capteurs
  Serial.begin(115200);
  Wire.begin();
  dht.begin();
  bmp.begin();

  // Initialisation du réseau Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  delay(200);
  
  while (WiFi.status() != WL_CONNECTED){
    digitalWrite(LEDWARN, HIGH);
    delay(300);
    digitalWrite(LEDWARN, LOW);
    delay(300);
  }
  digitalWrite(LEDWARN, LOW);
  digitalWrite(LEDOK , HIGH);
  
  // Configuration de Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    signupOK = true;
    delay(300);
  }
  else{
    digitalWrite(LEDWARN, HIGH);
    delay(300);
  }

  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

/*
 * fonction loop() exécutée en continu
 * Elle vérifie si la connexion à Firebase est établie
 * envoie périodiquement les données des capteurs à la base de données Firebase en temps réel.
*/
void loop(){
  unsigned long currentMillis = millis();
  if (Firebase.ready() && signupOK && currentMillis - previousMillis >= 20000){
    digitalWrite(LEDOK, LOW);
    if (Firebase.RTDB.setFloat(&fbdo, "DHT/Humidity", dht.readHumidity())){
      // OK!
    }
    else{
      //Err
      digitalWrite(LEDWARN, HIGH);
      delay(2000);
      Serial.println("err DHT");
      digitalWrite(LEDWARN, LOW);
      digitalWrite(LEDOK, HIGH);
    }
    if (Firebase.RTDB.setFloat(&fbdo, "DHT/Temperature", dht.readTemperature())){
      // OK!
    }
    else{
      //Err
      digitalWrite(LEDWARN, HIGH);
      delay(2000);
      Serial.println("err DHT");
      digitalWrite(LEDWARN, LOW);
      digitalWrite(LEDOK, HIGH);
    }
    if (Firebase.RTDB.setFloat(&fbdo, "BMP/Pressure", bmp.readPressure())){
      // OK!
    }
    else{
      //Err
      digitalWrite(LEDWARN, HIGH);
      delay(2000);
      Serial.println("err BMP");
      digitalWrite(LEDWARN, LOW);
      digitalWrite(LEDOK, HIGH);
    }
    if (Firebase.RTDB.setFloat(&fbdo, "BMP/Temperature", bmp.readTemperature())){
      // OK!
    }
    else{
      digitalWrite(LEDWARN, HIGH);
      delay(2000);
      Serial.println("err BMP");
      digitalWrite(LEDWARN, LOW);
      digitalWrite(LEDOK, HIGH);
    }
    if (Firebase.RTDB.setFloat(&fbdo, "PhotoCell/Lux", GetLux(analogRead(CAT_LUM)))){
      // OK!
    }
    else{
      //Err
      digitalWrite(LEDWARN, HIGH);
      delay(2000);
      Serial.println("err PhCell");
      digitalWrite(LEDWARN, LOW);
      digitalWrite(LEDOK, HIGH);
    }
    previousMillis = currentMillis;
  }
  digitalWrite(LEDOK, HIGH);
  delay(300);
}

/*
 * Calcule la luminosité en lux à partir de la valeur captée par le capteur
*/
int GetLux(int ValCapt){
  float Vout = float(ValCapt) * (3.3 / float(1024));
  float RLDR = (1000 * (3.3 - Vout))/Vout;
  int phys=500/(RLDR/1000);
  return phys * 20;
}
