
#ifndef TIMER_H_
#define TIMER_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"

/******************************************************************************
* Vid initiering sätts timerkretsar Timer 0 - 2 till att räkna upp med 
* prescaler 1024, vilket innebär en uppräkningsfrekvens på 16MHz/1024 , som motsvarar en
* uppräkningshastighet på 0.064ms. Därmed tar det 256 * 0.064ms = 16.384ms mellan
* varje timergenererat avbrott för en given timer när denna är aktiverad, då
* samtliga timerkretsar sätts till att räkna upp till 256, då avbrott sker och
* timerkretsen nollställs (automatiskt för Timer 0 samt Timer 2, då overflow
* sker för dessa 8-bitars timerkretsar, medan Timer 1 i stället nollställs av 
* hårdvaran i CTC Mode, då denna uppgår till 16-bitar och därmed är långt från
* full vid uppräkning till 256).
*
* För att initiera timerkrets Timer 0 med prescaler 1024 i Normal Mode så ettställs
* biten CS02 och CS00 (Clock Select 0 bit 2, 0 bit 0) i kontrollregistret TCCR0B (Timer/Counter
* Control Register 0B). Därmed sätts uppräkningsfrekvensen till 16/1024 MHz. 
*
* För att initiera timerkrets Timer 1 med prescaler 1024 i CTC Mode, så att avbrott 
* sker vid ett lägre värde än när overflow sker, vilket annars sker vid 
* uppräkning till 65 536 för en 16-bitars räknare, så ettställs bitar CS10, CS12 
* (Clock Select 1 bit 0 samt 2) samt WGM12 (Waveform Generator Mode 1 bit 3) i 
* kontrollregistret TCCR1B (Timer/Counter Control Register 1B). CS10 och CS12 används 
* för att ställa in uppräkningsfrekvensen umed prescaler 1024 (16 MHz/1024), medan WGM12 
* används för att Timer 1 skall arbeta i CTC Mode i stället för Normal Mode.
* För att ställa in att timergenererat avbrott skall ske på Timer 1 vid 
* uppräkning till 256, såsom övriga timerkretsar, så skrivs detta heltal till
* registret OCR1A (Output Compare Register 1A). Vid uppräkning till 256 sker
* då ett CTC-avbrott och Timer 1 nollställs av hårdvaran. Detta måste göras
* vid initiering av Timer 1.
*
* För att initiera timerkrets Timer 2 utan prescaler i Normal Mode så ettställs
* bitarna CS20, CS21, CS22 (Clock Select 2 bit 0, 1, 2) i kontrollregistret TCCR2B (Timer/Counter
* Control Register 2B). Därmed sätts uppräkningsfrekvensen till 16/1024 MHz, precis
* som för Timer 0. Dessa register uppgår båda till 8-bitar, vilket alltså
* medför overflow vid uppräkning till 256.
******************************************************************************/
#define INIT_TIMER0 TCCR0B = ((1<<CS00)|(1<<CS02)) /* Initierar Timer 0 i Normal Mode. */
#define INIT_TIMER1 TCCR1B = ((1<<CS10)|(1<<CS12)|(1<<WGM12)) /* Initierar Timer 1 i CTC Mode. */
#define INIT_TIMER2 TCCR2B = ((1<<CS20)|(1<<CS21)|(1<<CS22)) /* Initierar Timer 2 i Normal Mode. */

#define SET_TIMER1_LIMIT OCR1A = 256 /* CTC-avbrott för Timer 1 sker vid uppräkning till 256. */
#define INTERRUPT_TIME 16.384f /* 16.384 ms mellan timergenererade avbrott. */

/******************************************************************************
* För att aktivera Timer 0 i Normal Mode ettställs biten TOIE0 (Timer/Counter
* Overflow Interrupt Enable 0) i maskregistret TIMSK0 (Timer/Counter Mask
* Register 0). För att inaktivera avbrott nollställs i stället detta register.
* Avbrottsvektor för Timer 0 i Normal Mode är TIMER0_OVF_vect.
* 
* För att aktivera Timer 1 i CTC Mode så ettställs biten OCIE1A (Output Compare
* Interrupt Enable 1A) i maskregistret TIMSK1 (Timer/Counter Mask Register 1). 
* För att inaktivera avbrott nollställs i stället detta register.
* Avbrottsvektor för Timer 1 i CTC Mode är TIMER1_COMPA_vect.
* 
* För att aktivera Timer 2 i Normal Mode ettställs biten TOIE2 (Timer/Counter
* Overflow Interrupt Enable 2) i maskregistret TIMSK2 (Timer/Counter Mask
* Register 2). För att inaktivera avbrott nollställs i stället detta register.
* Avbrottsvektor för Timer 2 i Normal Mode är TIMER2_OVF_vect.
******************************************************************************/
#define ENABLE_TIMER0 ASSIGN(TIMSK0, TOIE0) /* Aktiverar Timer 0 i Normal Mode. */
#define ENABLE_TIMER1 ASSIGN(TIMSK1, OCIE1A) /* Aktiverar Timer 1 i CTC Mode. */
#define ENABLE_TIMER2 ASSIGN(TIMSK2, TOIE2) /* Aktiverar Timer 2 i Normal Mode. */

#define DISABLE_TIMER0 CLEAR(TIMSK0) /* Inaktiverar Timer 0 i Normal Mode. */
#define DISABLE_TIMER1 CLEAR(TIMSK1) /* Inaktiverar Timer 1 i CTC Mode. */
#define DISABLE_TIMER2 CLEAR(TIMSK2) /* Inaktiverar Timer 2 i Normal Mode. */

/******************************************************************************
* Strukten Timer används för att implementera mikrodatorns timerkretsar via
* timerobjekt. Mikrodatorns tre timerkretsar Timer 0 - 2 kan användas med
* valbar fördröjningstid. Samtliga timerkretsar räknar upp till 256 innan
* timergenererat avbrott sker, vilket medför avbrott var 16,384:e millisekund 
* för en given timer när denna är aktiverad (frekvensen sätts via respektive
* timers prescaler-bitar vid initieringen).
*
* Avbrottsvektorer för respektive timerkrets är följande:
*
* Timer 0: TIMER0_OVF_vect - Normal Mode.
* Timer 1: TIMER1_COMPA_vect - CTC Mode, maxvärde för uppräkning satt till 256.
* Timer 2: TIMER2_OVF_vect - Normal Mode.
******************************************************************************/
typedef struct Timer
{
	bool enabled; /* Indikerar ifall timern är aktiverad. */
	TimerSelection timerSelection; /* Använd timerkrets. */
	uint32_t executed_interrupts; /* Antalet avbrott som har ägt rum. */
	uint32_t required_interrupts; /* Antalet avbrott som krävs för aktuell fördröjning. */
} Timer;

/* Funktionsdeklarationer: */
Timer new_Timer(TimerSelection timerSelection, uint32_t delay_time);
void Timer_on(Timer* self);
void Timer_off(Timer* self);
void Timer_toggle(Timer* self);
void Timer_count(Timer* self);
bool Timer_elapsed(Timer* self);
void Timer_clear(Timer* self);
void Timer_reset(Timer* self);
void Timer_set(Timer* self, uint32_t delay_time);

#endif /* TIMER_H_ */