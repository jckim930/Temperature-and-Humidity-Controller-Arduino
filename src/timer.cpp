// Authors:
// Justin Kim, Spencer Kittredge, Christian Garcia, Yasser Alsaif        
// Net ID:
// jckim@email.arizona.edu, spencerkittredge@email.arizona.edu
// christiangarcia@email.arizona.edu, yalsaif@email.arizona.edu
// Date: 4/23/2019
// Assignment:     Final Project

#include "timer.h"

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
/*
TCCR0A and TCCR0B are control registers containing
WGM00, WGM01 for TCCR0A and WGM02 for TCCR0B
 0      1                       0 
 010 for CTC mode
*/
void initTimer0(){
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);  // these 3 lines enable CTC mode

    TCCR0B |= (1 << CS00) | (1 << CS01);
    TCCR0B &= ~(1 << CS02);  // These 2 lines enable

    OCR0A = 250;
}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-200 milliseconds
*/
void delayMs(unsigned int delay){
    TIFR0 |= (1 << OCF0A);
    TCNT0 = 0;
    unsigned int delayCount = 0;
    
    while (delayCount < delay) {
        if (TIFR0 & (1 << OCF0A)) { // checks the flag 
            delayCount++;
            TIFR0 |= (1 << OCF0A); // this line clears the flag
        }
    }
}
