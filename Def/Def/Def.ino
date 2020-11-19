#define BLYNK_PRINT Serial

#include <Wire.h>                   // Include library
#include <SPI.h>

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define light D0 //Light
DHT dht(D3, DHT22, 15); //Humid and Temp Sensor
int pinTone = D8; //sound
WidgetBridge bridge2(V2);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "g2b6CkHpEw2Wv9AusPg27WwuT_Nw7Wfn";
char auth[] = "fwod73-Fs-YJ-kuqDc1IPxoGoKTPCrLT";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "iotwifi";
char pass[] = "1234567890";

BlynkTimer timer;

  
  BLYNK_WRITE(V0){
  String pinValue = param.asString();
  Serial.print("Temp V0 Value is : ");
  Serial.println(pinValue);
  }

  BLYNK_WRITE(V1){
  int pinValue = param.asInt();
  Serial.print("Humid V1 Value is : ");
  Serial.println(pinValue);
  }

    BLYNK_WRITE(V2){
  int pinValue = param.asInt();
  Serial.print("Light V2 Value is : ");
  Serial.println(pinValue);
  }

    BLYNK_WRITE(V3){
  int pinValue = param.asInt();
  Serial.print("Distant V3 Value is : ");
  Serial.println(pinValue);
  }

    BLYNK_WRITE(V4){
  int pinValue = param.asInt();
  Serial.print("Sound switch V4 Value is : ");
  Serial.println(pinValue);
  }

void Sensor() {
  
  Serial.println("begin sensor");
  bridge2.virtualWrite(V2,1);

  ///////////////// Temp Humid/////////////////////////////
//  float h = dht.readHumidity();
//  float t = dht.readTemperature();
//  if (isnan(h) || isnan(t)) {
//    Serial.println("fail to begin sensor");
//    return;
//  }
//
//  Serial.printf("H=%.lf : T=%.lf ", h, t);
//  Serial.println("");
//
//  OLED.clearDisplay();               //Clear display
//  OLED.setTextColor(WHITE);          //Set text color
//  OLED.setCursor(0, 0);              //Set display start position
//  OLED.setTextSize(3);               //Set text size x1
//  OLED.println("Temp");          //Type message
//  OLED.setCursor(0, 35);            //Set display postion
//  OLED.println(String(t) + " C"); // Show result value
//  OLED.display();                    //Enable display
//
//
//  Blynk.virtualWrite(V0, t);
//  //    pin 1
//  WidgetLED led1(V1);
//  if (t > 25) {
//    Blynk.notify("oi oi Temp > 25");
//
//    led1.on();
//  } else {
//    led1.off();
//  }

}



void setup()
{
  // Debug console
  Serial.begin( 115200);
  pinMode(light, OUTPUT); //set Light
  dht.begin(); // set Humid and Temp Sensor
  Blynk.begin(auth, ssid, pass);

  while(Blynk.connect() == false){
    Serial.print("Connecting to B2");
    }
  
  timer.setInterval(2000L, Sensor);
  Wire.begin(); // need for read lux
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

BLYNK_CONNECTED(){
  bridge2.setAuthToken("fwod73-Fs-YJ-kuqDc1IPxoGoKTPCrLT");
  }

void loop()
{
  Blynk.run();
  timer.run();
}
