
#ifndef HEADER_H_
#define HEADER_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"
#include "GPIO.h" 
#include "Timer.h"
#include "Serial.h"
#include "ADC.h"

/* Globala variabler: */
struct Led* led1; 
Button button; 
struct Timer *timer0, *timer1; 
TempSensor tempSensor;

/* Funktionsdeklarationer: */
void setup(void);

#endif /* HEADER_H_ */