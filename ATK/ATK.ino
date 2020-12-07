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
DHT dht(D9, DHT22, 15); //Humid and Temp Sensor
int pinTone = D8; //sound
int BH1750address = 0x23;       // Set BH1750 address
int inputPin = D3;               // choose the input pin (for PIR sensor)
byte buff[2];
WidgetBridge bridge2(V2);

//Tew and PSU
//char auth[] = "Ar82EhKUaDi28TRffYK97YruWG58CYhc";//Authen code ATK
char ssid[] = "iotwifi";
char pass[] = "1234567890";

//ME
char auth[] = "Ven87jBqLrY-9DZ6wmxjbkUlADlD1u_W";//Authen code ATK
//char ssid[] = "uraiwan";
//char pass[] = "0817883998";

BlynkTimer timer;

BLYNK_WRITE(V4) {
  int pinValue = param.asInt();
  Serial.print("Sound switch V4 Value is : ");
  Serial.println(pinValue);
}

void Sensor() {
  Serial.println("begin sensor");
  ///////////////// Temp Humid/////////////////////////////
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("fail to begin sensor");
    return;
  }

  Serial.printf("H=%.lf : T=%.lf ", h, t);
  Serial.println("");

  //temp
  bridge2.virtualWrite(V0, t);
  //Humid
  bridge2.virtualWrite(V1, h);


  ///////////////////////////////Light lux////////////////////////

  uint16_t val = 0;
  BH1750_Init(BH1750address);             // Initial BH1750
  //  delay(200);
  if (2 == BH1750_Read(BH1750address))    // Read and check data from BH1750
  {
    val = ((buff[0] << 8) | buff[1]) / 1.2;
    Serial.println(String(val) + " lx"); // Show result value
    //light
    bridge2.virtualWrite(V2, val);
  } else {
    Serial.println("can't read lux"); // Show result value
  }

  //  Rat
  int valrat = digitalRead(inputPin);  // read input value
  Serial.println(valrat);
    //Rat
  bridge2.virtualWrite(V3, valrat);

  OLED.clearDisplay();               //Clear display
  OLED.setTextColor(WHITE);          //Set text color
  OLED.setCursor(0, 0);              //Set display start position
  OLED.setTextSize(1);               //Set text size x1
  OLED.print("Temp:");          //Type message
  OLED.print(t); // Show result value
  OLED.println(" C "); 
  OLED.print("Humid:");          //Type message
  OLED.println(h); // Show result value
  OLED.setCursor(0, 15);            //Set display postion
  OLED.print("Light:");          //Type message
  OLED.println(val); // Show result value
  OLED.print("Rat:");          //Type message
  OLED.println(valrat); // Show result value

}



void setup()
{
  // Debug console
  pinMode(inputPin, INPUT);     // set Dome Sendor
  Serial.begin(115200);
  pinMode(light, OUTPUT); //set Light
  dht.begin(); // set Humid and Temp Sensor
  Blynk.begin(auth, ssid, pass);

  while (Blynk.connect() == false) {
    Serial.print("Connecting to B2");
  }

  timer.setInterval(20L, Sensor);
  Wire.begin(); // need for read lux
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

BLYNK_CONNECTED() {
  //  bridge2.setAuthToken("CwfcDLtcKrAYdZQ96xeE3hE7QqacBGXz");//Authen code DEF
  bridge2.setAuthToken("XqWx_B489MGXJJ38uNtBs-Ub6IpRpvwL");//Authen code DEF me
}

void loop()
{
  Blynk.run();
  timer.run();
}

////Light Lux
int BH1750_Read(int address)              // BH1750 read data function
{
  int i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while (Wire.available())
  {
    buff[i] = Wire.read();                // Read one byte
    i++;
  }
  Wire.endTransmission();
  return i;
}
void BH1750_Init(int address)             // BH1750 initial function
{
  Wire.beginTransmission(address);
  Wire.write(0x10);                       // Start operation
  Wire.endTransmission();
}
