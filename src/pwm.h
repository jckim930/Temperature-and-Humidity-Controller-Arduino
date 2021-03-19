// Authors:
// Justin Kim, Spencer Kittredge, Christian Garcia, Yasser Alsaif        
// Net ID:
// jckim@email.arizona.edu, spencerkittredge@email.arizona.edu
// christiangarcia@email.arizona.edu, yalsaif@email.arizona.edu
// Date: 4/23/2019
// Assignment:     Final Project
#ifndef PWM_H
#define PWM_H

void initPWMTimer3();
void initPWMTimer4();

void changeDutyCycle3(float percent);
void changeDutyCycle4(float percent);

#endif