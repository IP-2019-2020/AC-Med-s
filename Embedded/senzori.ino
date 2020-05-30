#define USE_ARDUINO_INTERRUPTS true

#include <PulseSensorPlayground.h>
#include "Adafruit_Si7021.h"
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"
#include <NTPClient.h>
#include <WiFiUdp.h>


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
#define WIFI_SSID "Laviniu" 
#define WIFI_PASSWORD "keyboard" 

FirebaseData firebaseData;

const String cnp = "27788776674";
const String path = "/Date_Medicale_Senzor";

const int gas_sensor_limit = 70;
const int pulse_higher_limit = 100;
const int pulse_lower_limit = 70;

String Hours;
String Years;
String Gas_Alert;
String Pulse_Alert;

//GMT+3
const long utcOffsetInSeconds = 10800;
//Week Days
String weekDays[ 7 ] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[ 12 ] ={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


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

  timeClient.begin();
  
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
  pulse_signal_bpm = ( analogRead( ADC )/10 ) - rand()%30;

  if( ( pulse_signal_bpm > pulse_higher_limit ) || ( pulse_signal_bpm < pulse_lower_limit ) )
  {
    Pulse_Alert = "Pericol";
  }
  else
  {
    Pulse_Alert = "Parametrii normali";
  }
  digitalWrite( pulsePin, LOW );
  delay(500);
  digitalWrite( light_pin, HIGH );
  delay(500);
  light_value = analogRead( ADC );
  digitalWrite( light_pin, LOW );

  timeClient.update();
  int currentHour = timeClient.getHours();  
  int currentMinute = timeClient.getMinutes();

  String HM = String( currentHour ) + ":" + String( currentMinute );

  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;

  String currentDate = String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear);
  
  Firebase.setFloat(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Temperatura", temperatura);
  if( temperatura >= 40 )
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Temperatura " + "/Situatie",  "Temperatura ridicata");
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Temperatura " + "/Rezolvat",  "NU");
  }
  else
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Temperatura " + "/Situatie",  "Temperatura normala");
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Temperatura " + "/Rezolvat",  "Nu necesita rezolvare");
  }
  
  if( ( temperatura > 10 ) && ( temperatura < 20 ) )
  {
    if( ( umiditate >= 40 ) && ( umiditate <= 50 ) )
    {
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Umiditate", "Potrivita" );
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Situatie",  "Umiditatea  este in parametrii normali");
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Rezolvat",  "Nu necesita rezolvare");
    }
    else
    {
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Umiditate", "Nepotrivita" );
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Situatie",  "Umiditatea nu este in parametrii normali");
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Rezolvat",  "NU");
    }
  }
  else if( ( temperatura > 0 ) && ( temperatura < 10 ) )
  {
    if( ( umiditate >= 35 ) && ( umiditate <= 45 ) )
    {
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Umiditate", "Potrivita" );
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Situatie",  "Umiditatea  este in parametrii normali");
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Rezolvat",  "Nu necesita rezolvare");
    }
    else
    {
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Umiditate", "Nepotrivita" );
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Situatie",  "Umiditatea nu este in parametrii normali");
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Rezolvat",  "NU");
    };
  }
  else if( ( temperatura > -10 ) && ( temperatura < 0 ) )
  {
    if( ( umiditate >= 30 ) && ( umiditate <= 40 ) )
    {
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Umiditate", "Potrivita" );
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Situatie",  "Umiditatea  este in parametrii normali");
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Rezolvat",  "Nu necesita rezolvare");
    }
    else
    {
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Umiditate", "Nepotrivita" );
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Situatie",  "Umiditatea nu este in parametrii normali");
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Rezolvat",  "NU");
    };
  }
  else if( ( temperatura > 20 ) && ( temperatura < 30 ) )
  {
    if( ( umiditate >= 45 ) && ( umiditate <= 55 ) )
    {
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Umiditate", "Potrivita" );
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Situatie",  "Umiditatea  este in parametrii normali");
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Rezolvat",  "Nu necesita rezolvare");
    }
    else
    {
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Umiditate", "Nepotrivita" );
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Situatie",  "Umiditatea nu este in parametrii normali");
      Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Umiditate " + "/Rezolvat",  "NU");
    };
  }
  Firebase.setInt(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Activitate", pirStat);
  
  
  if( light_value < 10 )
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Lumina", "Intuneric" );
  }
  else if( ( light_value >= 10 ) && ( light_value < 200 ) ) 
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Lumina", "Intunecos" );
  }
  else if( ( light_value >= 200 ) && ( light_value < 500 ) ) 
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Lumina", "Lumina" );
  }
  else if( ( light_value >= 500 ) && ( light_value < 800 ) ) 
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Lumina", "Luminos" );
  }
  else
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Lumina", " Foarte luminos" );
  }

  
  if( senzor_gaz > gas_sensor_limit )
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Gaz", "Pericol, scurgeri de gaz." );
    Gas_Alert = "Pericol";
  }
  else
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Gaz", "Nu sunt scurgeri de gaz." );
    Gas_Alert = "Parametrii normali";
  }
  Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Gaz " + "/Situatie",  Gas_Alert);
  
  if( Gas_Alert.equals( "Pericol" ) )
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Gaz " + "/Rezolvat",  "NU");
  }
  else
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Gaz " + "/Rezolvat",  "Nu necesita rezolvare");
  }
  
  Firebase.setFloat(firebaseData, path + "/" + cnp + "/" + currentDate + "/" + HM + "/Pulse", pulse_signal_bpm );
  Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Puls " + "/Situatie", Pulse_Alert);
  
  if( Pulse_Alert.equals( "Pericol" ) )
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Puls " + "/Rezolvat", "NU");
  }
  else
  {
    Firebase.setString(firebaseData, path +  "/" + cnp + "/" + currentDate + "/" + HM + "/Alerte" + "/Alerta_Puls " + "/Rezolvat", "Nu necesita rezolvare");
  }
  
  delay(60000);

}
