// Authors:
// Justin Kim, Spencer Kittredge, Christian Garcia, Yasser Alsaif        
// Net ID:
// jckim@email.arizona.edu, spencerkittredge@email.arizona.edu
// christiangarcia@email.arizona.edu, yalsaif@email.arizona.edu
// Date: 4/23/2019
// Assignment:     Final Project
#include <avr/io.h>
#include "pwm.h"
#include <Arduino.h>


void initPWMTimer3() {

    DDRE |= 1 << DDE3; // Set Pin 5 (PE3) as output
    TCCR3A |= ((1 << WGM30) | (1 << COM3A1));
    TCCR3A &= ~((1 << COM3A0) | (1 << WGM31));

    // Prescaler set to 1
    TCCR3B |= ((1 << WGM32) | (1 << CS30));
    TCCR3B &= ~((1 << CS32) | (1 << CS31) | (1 << WGM33));

    OCR3A = 127; // 50% duty cycle
}

void initPWMTimer4() {
    DDRH |= 1 << DDH3; // Set Pin 6 (PH3) as output
    TCCR4A |= ((1 << COM4A1) | (1 << WGM40));
    TCCR4A &= ~((1 << COM4A0) | (1 << WGM41));

    // Prescaler set to 1
    TCCR4B |= ((1 << CS40) | (1 << WGM42));
    TCCR4B &= ~((1 << CS42) | (1 << CS41) | (1 << WGM43));

    OCR4A = 127;
}

void changeDutyCycle3(float percent) {
    OCR3A = int(percent * 0xFF);
}

void changeDutyCycle4(float percent) {
    OCR4A = int(percent * 0xFF);
}
