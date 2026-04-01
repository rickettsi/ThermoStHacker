//Sketch thermostatic chamber version started on september the 27th, 2022, 15h
//
//-----------Get following librairies

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LCD_I2C.h>

//------------Addressing of the IIC "slave" to pilot the LCD screen

LCD_I2C lcd(0x27, 16, 2);

//------------Attach "OneWire" bus to digital pin D2 of Arduino

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//------------Declaration of variable

float temp1; //1 measure of temp around the clario stacker
float temp2; //1 other measure of temp around the clario stacker
int temp_set = 0;
float targetted_temp;
float mean_temperature; //mean temp1 and temp2
float hysteresis_threshold = 0.15; //hysteresis of 0.3 °C, can be modified depending the experiment requirements
float targetted_temp_down; 
float targetted_temp_up;

void setup(){

//-------------Declaration of pin mode
  
  pinMode(8, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

//-------------Initialization of digital sensors and actuators
  
  lcd.begin();
  lcd.backlight();
  sensors.begin();
  Serial.begin(9600);
}

void loop(){

//---------------Modifiying targetted temperature

  lcd.setCursor(0, 0);
  temp_set = 1023 - analogRead(A0);//analog read of resistance of potentiometer
  targetted_temp = 25 + 0.01663*temp_set;//transformation of the read value into a temperature using a linear function
  targetted_temp = (long)(targetted_temp*10)/10.0;
  targetted_temp_down = targetted_temp - hysteresis_threshold;
//  targetted_temp_up = targetted_temp + hysteresis_threshold;

//---------------Displaying on lcd
    
  lcd.print("act_temp =");//print text
  lcd.setCursor(11, 0);//move the cursor
  lcd.print(mean_temperature);//print variable
  lcd.setCursor(0, 1);
  lcd.print("trg_temp =");//print text
  lcd.setCursor(11, 1);
  lcd.print(targetted_temp);//print variable

//----------------Thermosatic function

//----------------Measures of temperature in the heating chamber and around the stacker + security testing
  
sensors.requestTemperatures();
temp1 = sensors.getTempCByIndex(1);//
temp2 = sensors.getTempCByIndex(0);
mean_temperature = ((temp1+temp2)/2);

//----------------Regulation of clario_stacker chamber temperature

// (analogWrite(6, 255);) is the instruction to switch on the fan on the max speed,
// 6 is a pin index on arduino, the MOSFET (metal-oxyde semiconductor field-effect transistor) which controls the fan speed is plugged on the 6th digital pin !
//the speed can be set between 0% and 100% of max speed which corresponds to analogWrite(6, O) and analogWrite(6, 255), O% of max speed means the fan is turned off
//What a funny particularity of IT ! "percents" are coded on 8 bits, so percents are in fact values in [0;255]

// (digitalWrite(8,HIGH);)is the instruction to put digital pin 8 on HIGH level, that means pin 8 is +5V compared to the ground (GND pin)

if(mean_temperature < targetted_temp_down){
    digitalWrite(8, HIGH);
  }
else if(mean_temperature > targetted_temp){
    digitalWrite(8, LOW);
  }}
 //ideas to make it better :) : put a micro_sd card reader/writer to keep a history of measured temperatures
