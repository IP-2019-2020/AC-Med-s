#define USE_ARDUINO_INTERRUPTS true

#include <PulseSensorPlayground.h>
#include "Adafruit_Si7021.h"
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"


#define bound 115200
#define light_pin 14
#define pirPin  13                 // PIR Out pin 
#define gazPin  0
#define pulsePin 2
#define ADC A0
#define pulseBlink LED_BUILTIN
#define THRESHOLD 550

int pirStat = 0;                // PIR status
float senzor_gaz;
float pulse_signal_bpm;
float light_value;

Adafruit_Si7021 sensorsi = Adafruit_Si7021();
float temperatura;
float umiditate;

PulseSensorPlayground pulseSensor;

#define FIREBASE_HOST "medicalapp-36d7a.firebaseio.com" 
#define FIREBASE_AUTH "LsX4OkwTR6Mn9uZunlJFzf0lOPNZEPyaFgr7Opsa" 
#define WIFI_SSID "iPhone - Laviniu" 
#define WIFI_PASSWORD "gicutza77" 

FirebaseData firebaseData;

const String cnp = "17788776674";
const String path = "/Date_Medicale_Senzor";
void setup()
{
  Serial.begin( bound );
  sensorsi.begin();
  
  pinMode( pirPin, INPUT );
  pinMode( gazPin, OUTPUT );
  pinMode( pulsePin, OUTPUT );
  pinMode( light_pin, OUTPUT );
  pinMode( ADC, INPUT );
  
  digitalWrite( light_pin, LOW );
  digitalWrite( gazPin, LOW );
  digitalWrite( pulsePin, LOW );
  digitalWrite( light_pin, LOW );

  Serial.print("connecting to ");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void loop()
{

  temperatura = sensorsi.readTemperature();
  umiditate = sensorsi.readHumidity();
  pirStat = digitalRead( pirPin );
  digitalWrite( gazPin, HIGH );
  delay(500);
  senzor_gaz = analogRead( ADC );
  digitalWrite( gazPin, LOW );
  delay(500);
  digitalWrite( pulsePin, HIGH );
  delay(500);
  pulse_signal_bpm = analogRead( ADC );
  digitalWrite( pulsePin, LOW );
  delay(500);
  digitalWrite( light_pin, HIGH );
  delay(500);
  light_value = analogRead( ADC );
  digitalWrite( light_pin, LOW );
  
  Firebase.setFloat(firebaseData, path + "/" + cnp + "/Pulse", pulse_signal_bpm );
  Firebase.setFloat(firebaseData, path +  "/" + cnp + "/Temperatura", temperatura);
  Firebase.setFloat(firebaseData, path +  "/" + cnp + "/Umiditate", umiditate);
  Firebase.setInt(firebaseData, path +  "/" + cnp + "/Activitate", pirStat);
  Firebase.setFloat(firebaseData, path +  "/" + cnp + "/Gaz", senzor_gaz);
  Firebase.setFloat(firebaseData, path +  "/" + cnp + "/Lumina", light_value);

  delay(1000);

}
