
#ifndef HEADER_H_
#define HEADER_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"
#include "GPIO.h" 
#include "Timer.h"
#include "Serial.h"
#include "ADC.h"
#include "DynamicTimer.h"
/* Globala variabler: */
Led led1; 
Button button; 
Timer timer0;
DynamicTimer timer1; 
TempSensor tempSensor;

/* Funktionsdeklarationer: */
void setup(void);

#endif /* HEADER_H_ */