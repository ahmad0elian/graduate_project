#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL67paEGDnf"
#define BLYNK_TEMPLATE_NAME "danger"
#define BLYNK_AUTH_TOKEN "hz0zCngydOa0QQeL3157b942HaG8NZKm"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int arduinoPin = 2;
int danger;

char ssid[] = "Shadow_Gtx";  // this must be exactly like the name of your wifi network
char pass[] = "osama2001";

BlynkTimer timer; 

void myTimer() {
  Blynk.virtualWrite(V0, danger);
}

void setup()
{
  Serial.begin(115200);
  pinMode(arduinoPin, INPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(300L, myTimer); 
}

void loop()
{
  Blynk.run();
  timer.run();
  
  danger = digitalRead(arduinoPin);
  Serial.println(danger);
}
