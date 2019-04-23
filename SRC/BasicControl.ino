/* Demonstration sketch for PCF8574T I2C LCD Backpack 
Uses library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads GNU General Public License, version 3 (GPL-3.0) */
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define DHTPIN 5         // DHT data pin connected to Arduino pin 5
#define DHTTYPE DHT22    // DHT 22 (if your sensor is the DHT 11, only change this line by: #define DHTTYPE DHT11) 
#define LDR_PIN 0        // used for Luminosity (LDR) Sensor Input
#define SOIL_MOIST_PIN 1 // used for Soil Moisture Sensor Input
#define SMS_VCC 7 // used for Soil Moisture Sensor "Power Supply" (VCC)
#define SMS_GND 6 // used for Soil Moisture Sensor "GND"
// Actuators: Buttons and LEDs
#define PUMP_ON 11  //push-button
#define PUMP_PIN 8


int led_pin = 13;

//variables
int tempDHT; 
int humDHT;
int lumen;
int soilMoist;
// Variables to be used by Actuators
boolean pumpStatus = 0;

// Variables to be used with timers
long sampleTimingSeconds = 300; // ==> Define Sample time in seconds to read sensores
long startTiming = 0;
long elapsedTime = 0;

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE); 
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified backpack


//function 
/* Receive Commands and act on actuators
****************************************************/
void aplyCmd()
{
    if (pumpStatus == 1) digitalWrite(PUMP_PIN, HIGH);
    if (pumpStatus == 0) digitalWrite(PUMP_PIN, LOW);
}
void showDataLCD(void)
{
  lcd.setCursor (0,0);
  lcd.print("Tem:");
  lcd.print(tempDHT);
  lcd.print("oF Hum:");
  lcd.print(humDHT);
  lcd.print("%  ");
  lcd.setCursor (0,1);
  lcd.print("Lig:");
  lcd.print(lumen);
  lcd.print("%");
  lcd.print(" Soi:");
  lcd.print(soilMoist);
  lcd.print("%");
}
void readLocalCmd() 
{  
  int digiValue = debounce(PUMP_ON);
  if (!digiValue) 
  {
    pumpStatus = !pumpStatus;
    showDataLCD();
    aplyCmd();
  }
}
void readSensors()
{
  tempDHT = dht.readTemperature();   //Read temperature and humidity values from DHT sensor:
  humDHT = dht.readHumidity();
  lumen = getLumen(LDR_PIN);
  soilMoist = getSoilMoist();
}
int getLumen(int anaPin)
{
  int anaValue = 0;
  for(int i = 0; i < 10; i++) // read sensor 10X ang get the average
  {
    anaValue += analogRead(anaPin);   
    delay(50);
  }
  
  anaValue = anaValue/10; //Light under 300; Dark over 800
  anaValue = map(anaValue, 1023, 0, 0, 100); //LDRDark:0  ==> light 100%

  return anaValue;  
}
int getSoilMoist()
{
  int sensorValue = analogRead(SOIL_MOIST_PIN);
  return sensorValue;
}
void printData(void)
{
  Serial.print("   Temp DHT ==> ");
  Serial.print(tempDHT);
  Serial.print("oC  Hum DHT ==> ");
  Serial.print(humDHT);
  Serial.print("%  Luminosity ==> ");
  Serial.print(lumen);
  Serial.print("%  Soil Moisture ==> ");
  Serial.print(soilMoist);
  Serial.print("%  Pump: ");
  Serial.print(pumpStatus);
}
/***************************************************
* Showing capured data at LCD
****************************************************/

/***************************************************
* Debouncing a key
****************************************************/
boolean debounce(int pin)
{
  boolean state;
  boolean previousState;
  const int debounceDelay = 60;
  
  previousState = digitalRead(pin);
  for(int counter=0; counter< debounceDelay; counter++)
  {
    delay(1);
    state = digitalRead(pin);
    if(state != previousState)
    {
      counter =0;
      previousState = state;
    } 
  }
 return state;
}



void setup()
{
  // activate LCD module
  Serial.begin(9600);
  // begin LCD
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  // begin DHT
  dht.begin();
  //define pinMode
  pinMode(SOIL_MOIST_PIN , INPUT);
  pinMode(PUMP_ON, INPUT_PULLUP); // enabling internal pull-up resistor, button 
  pinMode(PUMP_PIN,OUTPUT);// relay into 
  pinMode(led_pin,OUTPUT);  // relay control 
  digitalWrite(led_pin,HIGH);
  readSensors(); // innitial reading
  startTiming = millis(); // starting the "program clock"
  }
  
  

void loop()
{
  //relay
  //int val = digitalRead(2);
 // Serial.println(val);
 
 // if (val == LOW) {
  //  digitalWrite(relay_pin, HIGH);
 // } else {
  //  digitalWrite(relay_pin, LOW);
  //}
   
 // lcd.home (); // set cursor to 0,0
 // lcd.setBacklight(HIGH);
 // lcd.setCursor (0,0);        // go to start of 2nd line
 // Serial.println("quang tro la: ");
 // int A_Light = Monitor_Light();
 // int Soil_Moisture = Monitor_Soil(); 
 // lcd.print("Lit:");
 // lcd.print(A_Light);
 // lcd.print(" ");
 // Serial.println(A_Light);
 // lcd.print("Soi:");
 // lcd.print(Soil_Moisture); 
 // Serial.println("do am dat la: ");
 // Serial.println(Soil_Moisture);
 // delay(2000);

 // lcd.setCursor (0,1);

 // delay(2000);
  // Start timer for measurements
  elapsedTime = millis()-startTiming; 
  readLocalCmd(); //Read local button status
  showDataLCD();
  printData();
   if (elapsedTime > (sampleTimingSeconds*1000)) 
  {
    readSensors();
    startTiming = millis();
     
  }
}

