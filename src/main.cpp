#include <Arduino.h>
#include <avr/io.h>
#include "i2c.h"
#include "timer.h"
#include <math.h>
#include "pwm.h"
#include "led.h"
// Authors:
// Justin Kim, Spencer Kittredge, Christian Garcia, Yasser Alsaif        
// Net ID:
// jckim@email.arizona.edu, spencerkittredge@email.arizona.edu
// christiangarcia@email.arizona.edu, yalsaif@email.arizona.edu
// Date: 4/23/2019
// Assignment:     Final Project
/*
Trigger Temperatue Measurement  0xE3    - Hold Master
Trigger Humidity Measurement    0xE5    - Hold Master
Trigger Temperature Measurement 0xF3    - No Hold Master
Trigger Humidity Measurement    0xF5    - No Hold Master
Write User Register             0xE6
Read User Register              0xE7
Soft Reset                      0xFE
*/

/*
Pin 2 - PE4 heating element
Pin 3 - PE5 cooling element
Pin 4 - PG5 humidifying element
*/

// Pin 5 - PWM 

#define i2cAddress 0x40
#define i2cWrite 0x80
#define i2cRead 0x81
#define wait_for_completion while(!(TWCR & (1 << TWINT)));


int main(void) {
  Serial.begin(9600);
  Serial.flush();
  initTimer0();
  initI2C();
  initPWMTimer3();
  initLED();
  
  delayMs(15);
  
  bool led2 = false; // heater
  bool led3 = false; // cooler
  bool led4 = false; // humidifier
  double maxTemp = 25.5; // degrees Celsius
  double minTemp = 24.3; 
  double minHumidity = 35.00;
  double maxHumidity = 47.40; // % relative humidity
  double temp;
  double humidity;
  unsigned int tempData;
  unsigned int humidityData;
  unsigned int checksum;

  //Soft Reset
  beginTransmission(i2cWrite);
  write(0xFE);
  endTransmission();
  delayMs(15);
  while(1) {
    beginTransmission(i2cWrite);
    write(0xE3); // measure temp
    beginTransmission(i2cRead);
    delayMs(50);
    tempData = read();
    checksum = readChecksum();
    endTransmission();

    tempData &= 0b1111111111111100; // Set bits 0 and 1 to 0, they're the status bits
    temp = (175.72 * (tempData / 65536.0)) - 46.85; // convert data to temperature in Celsius
    Serial.print("temp: ");
    Serial.println(temp);

    beginTransmission(i2cWrite);
    write(0xE5); // measure humidity
    beginTransmission(i2cRead);
    delayMs(50);
    humidityData = read();
    checksum = readChecksum();
    endTransmission();

    humidityData &= 0b1111111111111100; // Set bits 0 and 1 to 0, they're the status bits
    humidity = (125.0 * (tempData / 65536.0)) - 6.0; // convert data to temperature in Celsius
    Serial.print("humidity: ");
    Serial.println(humidity);

    if (temp <= minTemp) { // turn on the heating device
      turnOnLED(2);
      led2 = true;
      turnOffLED(3);
      led3 = false;
    } 
    else if (temp >= maxTemp) { // turn on the cooling device
      turnOnLED(3);
      led3 = true;
      turnOffLED(2);
      led2 = false;
    } 
    else { // if in normal operating range, turn off both heating and cooling devices
      turnOffLED(2);
      led2 = false;
      turnOffLED(3);
      led3 = false;
    }

    if (humidity >= maxHumidity || humidity <= minHumidity) {
      turnOnLED(4);
      led4 = true;
    } 
    else {
      turnOffLED(4);
      led4 = false;
    }

    // turn on the fan if any of the LEDs are on
    if (led2 || led3 || led4) {
      changeDutyCycle3(80);
    } 
    else {
      changeDutyCycle3(0);
    }

    Serial.print("temp: ");
    Serial.println(temp);
    Serial.print("humidity: ");
    Serial.println(humidity);
    Serial.print("led2: ");
    Serial.println(led2);
    Serial.print("led3: ");
    Serial.println(led3);
    Serial.print("led4: ");
    Serial.println(led4);

    delayMs(1500);
  }

  return 1;
}

