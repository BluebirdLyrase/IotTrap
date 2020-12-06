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

#define swt D7
DHT dht(D3, DHT22, 15); //Humid and Temp Sensor
int pinTone = D8; //sound
WidgetBridge bridge2(V2);

//Tew and PSU
//char auth[] = "CwfcDLtcKrAYdZQ96xeE3hE7QqacBGXz";//Authen code 1
//char ssid[] = "iotwifi";
//char pass[] = "1234567890";


//ME
char auth[] = "XqWx_B489MGXJJ38uNtBs-Ub6IpRpvwL";//Authen code DEF
char ssid[] = "uraiwan";
char pass[] = "0817883998";

int temp;
int humid;
int light;
int rat;

BlynkTimer timer;
  
  BLYNK_WRITE(V0){
  int pinValue = param.asInt();
  Serial.print("Temp V0 Value is : ");
  Serial.println(pinValue);
  temp = pinValue;
  }

  BLYNK_WRITE(V1){
  int pinValue = param.asInt();
  Serial.print("Humid V1 Value is : ");
  Serial.println(pinValue);
  humid = pinValue;
  }

  BLYNK_WRITE(V2){
  int pinValue = param.asInt();
  Serial.print("Light V2 Value is : ");
  Serial.println(pinValue);
  light = pinValue;
  }

  BLYNK_WRITE(V3){
  int pinValue = param.asInt();
  Serial.print("Rat V3 Value is : ");
  Serial.println(pinValue);
  rat = pinValue;
  }

void Sensor() {
  
  Serial.println("begin sensor");
//  bridge2.virtualWrite(V2,1);
//  bool out = digitalRead(swt);
  Serial.println(temp + humid + light + rat);
  OLED.clearDisplay();               //Clear display
  OLED.setTextColor(WHITE);          //Set text color
  OLED.setCursor(0, 0);              //Set display start position
  OLED.setTextSize(1);               //Set text size x1
  OLED.print("Temp:");          //Type message
  OLED.print(temp); // Show result value
  OLED.println(" C "); 
  OLED.print("Humid:");          //Type message
  OLED.println(humid); // Show result value
  OLED.setCursor(0, 15);            //Set display postion
  OLED.print("Light:");          //Type message
  OLED.println(light); // Show result value
  OLED.print("Rat:");          //Type message
  OLED.println(rat); // Show result value

  OLED.display();                    //Enable display
}
void setup()
{
  // Debug console
  Serial.begin( 115200);
  pinMode(light, OUTPUT); //set Light
  dht.begin(); // set Humid and Temp Sensor
  Blynk.begin(auth, ssid, pass);
  pinMode(swt,INPUT);

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
//  bridge2.setAuthToken("Ar82EhKUaDi28TRffYK97YruWG58CYhc");//Authen ATF 
  bridge2.setAuthToken("Ven87jBqLrY-9DZ6wmxjbkUlADlD1u_W");//Authen ATF me
  }

void loop()
{
  Blynk.run();
  timer.run();
}
