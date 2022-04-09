
#ifndef TIMER_H_
#define TIMER_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"

/******************************************************************************
* Vid initiering s�tts timerkretsar Timer 0 - 2 till att r�kna upp utan
* prescaler, vilket inneb�r en uppr�kningsfrekvens p� 16 MHz, som motsvarar en
* uppr�kningshastighet p� 62.5 us. D�rmed tar det 256 * 62.5u = 0.016 ms mellan
* varje timergenererat avbrott f�r en given timer n�r denna �r aktiverad, d�
* samtliga timerkretsar s�tts till att r�kna upp till 256, d� avbrott sker och
* timerkretsen nollst�lls (automatiskt f�r Timer 0 samt Timer 2, d� overflow
* sker f�r dessa 8-bitars timerkretsar, medan Timer 1 i st�llet nollst�lls av 
* h�rdvaran i CTC Mode, d� denna uppg�r till 16-bitar och d�rmed �r l�ngt fr�n
* full vid uppr�kning till 256).
*
* F�r att initiera timerkrets Timer 0 utan prescaler i Normal Mode s� ettst�lls
* biten CS00 (Clock Select 0 bit 0) i kontrollregistret TCCR0B (Timer/Counter
* Control Register 0B). D�rmed s�tts uppr�kningsfrekvensen till 16 MHz. 
*
* F�r att initiera timerkrets Timer 1 utan prescaler i CTC Mode, s� att avbrott 
* sker vid ett l�gre v�rde �n n�r overflow sker, vilket annars sker vid 
* uppr�kning till 65 536 f�r en 16-bitars r�knare, s� ettst�lls bitar CS10 
* (Clock Select 1 bit 0) samt WGM11 (Waveform Generator Mode 1 bit 1) i 
* kontrollregistret TCCR1B (Timer/Counter Control Register 1B). CS10 anv�nds 
* f�r att st�lla in uppr�kningsfrekvensen utan prescaler (16 MHz), medan WGM12 
* anv�nds f�r att Timer 1 skall arbeta i CTC Mode i st�llet f�r Normal Mode.
* F�r att st�lla in att timergenererat avbrott skall ske p� Timer 1 vid 
* uppr�kning till 256, s�som �vriga timerkretsar, s� skrivs detta heltal till
* registret OCR1A (Output Compare Register 1A). Vid uppr�kning till 256 sker
* d� ett CTC-avbrott och Timer 1 nollst�lls av h�rdvaran. Detta m�ste g�ras
* vid initiering av Timer 1.
*
* F�r att initiera timerkrets Timer 2 utan prescaler i Normal Mode s� ettst�lls
* biten CS20 (Clock Select 2 bit 0) i kontrollregistret TCCR2B (Timer/Counter
* Control Register 2B). D�rmed s�tts uppr�kningsfrekvensen till 16 MHz, precis
* som f�r Timer 0. Dessa register uppg�r b�da till 8-bitar, vilket allts�
* medf�r overflow vid uppr�kning till 256.
******************************************************************************/
#define INIT_TIMER0 ASSIGN(TCCR0B, CS00) /* Initierar Timer 0 i Normal Mode. */
#define INIT_TIMER1 ASSIGN(TCCR1B, WGM11) /* Initierar Timer 1 i CTC Mode. */
#define INIT_TIMER2 ASSIGN(TCCR2B, CS20) /* Initierar Timer 2 i Normal Mode. */

#define SET_TIMER1_LIMIT OCR1A = 256 /* CTC-avbrott f�r Timer 1 sker vid uppr�kning till 256. */
#define INTERRUPT_TIME 0.016f /* 0.016 ms mellan timergenererade avbrott. */

/******************************************************************************
* F�r att aktivera Timer 0 i Normal Mode ettst�lls biten TOIE0 (Timer/Counter
* Overflow Interrupt Enable 0) i maskregistret TIMSK0 (Timer/Counter Mask
* Register 0). F�r att inaktivera avbrott nollst�lls i st�llet detta register.
* Avbrottsvektor f�r Timer 0 i Normal Mode �r TIMER0_OVF_vect.
* 
* F�r att aktivera Timer 1 i CTC Mode s� ettst�lls biten OCIE1A (Output Compare
* Interrupt Enable 1A) i maskregistret TIMSK1 (Timer/Counter Mask Register 1). 
* F�r att inaktivera avbrott nollst�lls i st�llet detta register.
* Avbrottsvektor f�r Timer 1 i CTC Mode �r TIMER1_COMPA_vect.
* 
* F�r att aktivera Timer 2 i Normal Mode ettst�lls biten TOIE2 (Timer/Counter
* Overflow Interrupt Enable 2) i maskregistret TIMSK2 (Timer/Counter Mask
* Register 2). F�r att inaktivera avbrott nollst�lls i st�llet detta register.
* Avbrottsvektor f�r Timer 2 i Normal Mode �r TIMER2_OVF_vect.
******************************************************************************/
#define ENABLE_TIMER0 ASSIGN(TIMSK0, TOIE0) /* Aktiverar Timer 0 i Normal Mode. */
#define ENABLE_TIMER1 ASSIGN(TIMSK1, TOIE1) /* Aktiverar Timer 1 i CTC Mode. */
#define ENABLE_TIMER2 ASSIGN(TIMSK2, TOIE2) /* Aktiverar Timer 2 i Normal Mode. */

#define DISABLE_TIMER0 CLEAR(TIMSK0) /* Inaktiverar Timer 0 i Normal Mode. */
#define DISABLE_TIMER1 CLEAR(TIMSK0) /* Inaktiverar Timer 1 i CTC Mode. */
#define DISABLE_TIMER2 CLEAR(TIMSK2) /* Inaktiverar Timer 2 i Normal Mode. */

/******************************************************************************
* Strukten Timer anv�nds f�r att implementera mikrodatorns timerkretsar via
* timerobjekt. Mikrodatorns tre timerkretsar Timer 0 - 2 kan anv�ndas med
* valbar f�rdr�jningstid. Samtliga timerkretsar r�knar upp till 256 innan
* timergenererat avbrott sker, vilket medf�r avbrott var 1.024:e millisekund 
* f�r en given timer n�r denna �r aktiverad (frekvensen s�tts via respektive
* timers prescaler-bitar vid initieringen).
*
* Avbrottsvektorer f�r respektive timerkrets �r f�ljande:
*
* Timer 0: TIMER0_OVF_vect - Normal Mode.
* Timer 1: TIMER1_COMPA_vect - CTC Mode, maxv�rde f�r uppr�kning satt till 256.
* Timer 2: TIMER2_OVF_vect - Normal Mode.
******************************************************************************/
struct Timer
{
	enum bool enabled; /* Indikerar ifall timern �r aktiverad. */
	enum TimerSelection timerSelection; /* Anv�nd timerkrets. */
	unsigned long executed_interrupts; /* Antalet avbrott som har �gt rum. */
	unsigned long required_interrupts; /* Antalet avbrott som kr�vs f�r aktuell f�rdr�jning. */
};

/* Funktionsdeklarationer: */
struct Timer* new_Timer(enum TimerSelection* timerSelection, float* delay_time);
void Timer_on(struct Timer* self);
void Timer_off(struct Timer* self);
void Timer_toggle(struct Timer* self);
void Timer_count(struct Timer* self);
enum bool Timer_elapsed(struct Timer* self);
void Timer_clear(struct Timer* self);
void Timer_reset(struct Timer* self);
void Timer_set(struct Timer* self, float* delay_time);
void init_timer(enum TimerSelection* timerSelection);
unsigned long get_required_interrupts(float* delay_time);

#endif /* TIMER_H_ */