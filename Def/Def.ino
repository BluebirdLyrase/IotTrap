#define BLYNK_PRINT Serial
#include <TridentTD_LineNotify.h>
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


#define redlight D7 //Light
#define yellowlight D6 //Light
#define btn D5
DHT dht(D3, DHT22, 15); //Humid and Temp Sensor
WidgetBridge bridge2(V2);

//Tew and PSU
//char auth[] = "CwfcDLtcKrAYdZQ96xeE3hE7QqacBGXz";//Authen code 1
char ssid[] = "iotwifi";
char pass[] = "1234567890";

//char ssid[] = "moi8lite";
//char pass[] = "12341234";


//ME
char auth[] = "XqWx_B489MGXJJ38uNtBs-Ub6IpRpvwL";//Authen code DEF
//char ssid[] = "uraiwan";
//char pass[] = "0817883998";
String LINE_TOKEN = "5VlLmYvoiehckgOpxJMLfLSoLnnPoWk2ahHbjPDf3RA";   //Line Token

int temp;
int humid;
int light;
int rat;
int ratCount;
boolean isNew = true ;

BlynkTimer timer;

BLYNK_WRITE(V0) {
  int pinValue = param.asInt();
//  Serial.print("Temp V0 Value is : ");
//  Serial.println(pinValue);
  temp = pinValue;
}

BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
//  Serial.print("Humid V1 Value is : ");
//  Serial.println(pinValue);
  humid = pinValue;
}

BLYNK_WRITE(V2) {
  int pinValue = param.asInt();
//  Serial.print("Light V2 Value is : ");
//  Serial.println(pinValue);
  light = pinValue;
}

BLYNK_WRITE(V3) {
  int pinValue = param.asInt();
//  Serial.print("Rat V3 Value is : ");
//  Serial.println(pinValue);
  rat = pinValue;
}

void Sensor() {
  //  bridge2.virtualWrite(V2,1);
  //  bool out = digitalRead(swt);
//  Serial.println(temp + humid + light + rat);
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
//  
//  Blynk.virtualWrite(V0,temp); 
//  Blynk.virtualWrite(V1,humid);



  if (rat > 0) {
    digitalWrite(redlight, HIGH);
    if(isNew){
    ratCount = ratCount + 1;
     LINE.notify("Caught a rat!");
    }
    isNew = false;
  }
//    Serial.println(ratCount);
    Blynk.virtualWrite(V4,ratCount);

  int buttonstate = digitalRead(btn);
//  Serial.println(buttonstate);

  ///Button Press
  if (buttonstate == LOW) {
    digitalWrite(redlight, LOW);
    isNew = true;
    LINE.notify("Cage Reset");
  }

  //
  if (temp >= 18 && temp <= 27 && humid >= 30 && humid <= 70) {
    digitalWrite(yellowlight, HIGH);
  } else {
    digitalWrite(yellowlight, LOW);
  }

  OLED.print("Rat :");          //Type message
  OLED.println(ratCount); // Show result value
  
  if(!isNew){ // isNew is basicly reverse have rat 
    OLED.println("status : HAVE RAT!"); // Show result value
  }else{
    OLED.println("status : NO RAT"); // Show result value
  }

  //for test
  //  if(temp>=18&&temp<=30&&humid>=69&&humid<=70){
  //    digitalWrite(yellowlight, HIGH);
  //    }else{
  //      digitalWrite(yellowlight, LOW);
  //      }

  //  digitalWrite(yellowlight, HIGH);

  OLED.display();                    //Enable display
}
void setup()
{
  // Debug console
  Serial.begin(115200);
  LINE.setToken(LINE_TOKEN);
  pinMode(redlight, OUTPUT); //set Light
  pinMode(yellowlight, OUTPUT); //set Light
  pinMode(btn, INPUT); //redlight toggle btn
  dht.begin(); // set Humid and Temp Sensor
  Blynk.begin(auth, ssid, pass);

  while (Blynk.connect() == false) {
    Serial.print("Connecting to B2");
  }

  timer.setInterval(2000L, Sensor);
  Wire.begin(); // need for read lux
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

BLYNK_CONNECTED() {
  //  bridge2.setAuthToken("Ar82EhKUaDi28TRffYK97YruWG58CYhc");//Authen ATF
  bridge2.setAuthToken("Ven87jBqLrY-9DZ6wmxjbkUlADlD1u_W");//Authen ATF me
}

void loop()
{
  Blynk.run();
  timer.run();
}
