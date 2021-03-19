// Authors:
// Justin Kim, Spencer Kittredge, Christian Garcia, Yasser Alsaif        
// Net ID:
// jckim@email.arizona.edu, spencerkittredge@email.arizona.edu
// christiangarcia@email.arizona.edu, yalsaif@email.arizona.edu
// Date: 4/23/2019
// Assignment:     Final Project
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
#include "timer.h"

 #define LONG_DELAY 200
 #define SHORT_DELAY 100
/*

Pin 2 - PE4 heating element
Pin 3 - PE5 cooling element
Pin 4 - PG5 humidifying element


DDxn - 0 (input) ()
     - 1 (output)

PORTxn - Output PORT register (drives the PIN high or low)
                                                1       0
        PORT is what you write 
PINxn - Input PORT register (reads in a value)
*/

// initLED which returns void and takes no parameters
void initLED(){
    DDRG |= 1<<DDG5; // Pin 4
    DDRE |= ((1<<DDE4) | (1<<DDE5)); // 2, 3
}

// turnOnLED which returns void and takes in an unsigned int parameter called led
void turnOnLED(unsigned int led){
    switch(led) {
        case 2:
            PORTE |= 1<<PORTE4;
            break;
        case 3:
            PORTE |= 1<<PORTE5;
            break;
        case 4:
             PORTG |= 1<<PORTG5;
             break;
        default: // do nothing
            break;
    }
}

// turnOffLED which returns void and takes in an unsigned int parameter called led
void turnOffLED(unsigned int led){
    switch(led) {
        case 2:
            PORTE &= ~(1 << PORTE4);
            break;
        case 3:
            PORTE &= ~(1 << PORTE5);
            break;
        case 4:
             PORTG &= ~(1 << PORTG5);
             break;
        default: // do nothing
            break;
    }
}

// testLED which returns void and takes in an unsigned int parameter called led
void testLED(unsigned int led){
    if (led >= 2 && led <= 4) {
        turnOnLED(led);
        delayMs(100);
        turnOffLED(led);
    }
}
