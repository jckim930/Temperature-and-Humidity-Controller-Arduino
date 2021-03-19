#include <avr/io.h>
#include "i2c.h"
#include <Arduino.h>
// Authors:
// Justin Kim, Spencer Kittredge, Christian Garcia, Yasser Alsaif        
// Net ID:
// jckim@email.arizona.edu, spencerkittredge@email.arizona.edu
// christiangarcia@email.arizona.edu, yalsaif@email.arizona.edu
// Date: 4/23/2019
// Assignment:     Final Project

/*
SCL - clock
SDA - data

Start condition:
SCL HIGH and SDA goes HIGH to LOW

Data bits transferrerd (B1...BN)
A bit is transmitted at every high level of SCL after start condition

Stop bit (P)
To stop the data transfer, SCL is held high, wile SDA goes from LOW to HIGH

Data never changes when the click is HIGH
Data changes while the clock is LOW, and is stable wile the clock is HIG
*/

/*
I2C Master Read

1. Send a start sequence
2. Send the I2C address of the sensor module with the R/W bit low
3. Send the Internal address of the sensor module that needs to be read
4. Send a start sequence again (repeated start)
5. Send I2C address of the sensor module with the R/W bit high
6. Read data byte from the sensor module
7. Master sends a Not Acknowledge (NACK)
8. Send the stop sequence
*/

/*
TWINT : Interrupt Flag      - 7
TWEA : Enable Acknowledge   - 6
TWSTA : Start Condition     - 5
TWSTO : Stop Condition      - 4
TWEN : I2C Enable           - 2
TWIE : Interrupt Enable     - 0
*/

/*
TWBR : Bit Rate Register
TWDR : Data Register
TWCR : Control Register
TWSR : Status Register
*/

//I2C Address 0x40 
// Checksum error detection 
// X^8 + X^5 + X^4 + 1
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
CHECKING STATUS REGISTER
  uint8_t i2c_status = TWSR & 0b11111100;
  Serial.println(i2c_status, HEX);
*/

#define wait_for_completion while(!(TWCR & (1 << TWINT)));

void initI2C() {
  TWSR |= (1 << TWPS0);
  TWSR &= ~(1 << TWPS1); // prescaler = 1
  TWBR = 0xC6; // bit rate generator, SCK is 10KHz
  
  TWCR |= (1 << TWINT | 1 << TWEN); // enable two wire interface
}

void write(unsigned int data) {
  TWDR = data;
  TWCR = ((1 << TWEN) | (1 << TWINT));
  wait_for_completion;
}

void beginTransmission(unsigned int address) {
  TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWSTA)); // Set Start Condition
  wait_for_completion;
  write(address);
}


// Multibyte read for the temperature/humidity
unsigned int read() {
  unsigned int data;
  TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWEA));
  wait_for_completion;
  data = (TWDR << 8);

  TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWEA));
  wait_for_completion;
  data += TWDR;

  return data;
}

unsigned int readChecksum() {
  unsigned int checksum;
  checksum = TWDR;
  TWCR = ((1 << TWEN) | (1 << TWINT)); // NOT ACK
  wait_for_completion;

  return checksum;
}

void endTransmission() {
  TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO)); // Send Stop condition
}