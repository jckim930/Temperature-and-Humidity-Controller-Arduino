// Authors:
// Justin Kim, Spencer Kittredge, Christian Garcia, Yasser Alsaif        
// Net ID:
// jckim@email.arizona.edu, spencerkittredge@email.arizona.edu
// christiangarcia@email.arizona.edu, yalsaif@email.arizona.edu
// Date: 4/23/2019
// Assignment:     Final Project

#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

void initI2C();
void write(unsigned int data);
void beginTransmission(unsigned int address);
unsigned int read();
unsigned int readChecksum();
void endTransmission();

#endif