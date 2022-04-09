
#ifndef GPIO_H_
#define GPIO_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"

/******************************************************************************
* Strukten Led anv�nds f�r implementering av lysdioder, som kan placeras p�
* n�gon av digitala PINs 0 - 13 p� Arduino Uno. Varje lysdiod kan t�ndas, 
* sl�ckas och togglas.
******************************************************************************/
struct Led
{
	unsigned char PIN; /* Aktuellt PIN-nummer. */
	enum bool enabled; /* Indikerar ifall lysdioden �r p� eller inte. */
	enum IO_port io_port; /* I/O-port som lysdioden �r ansluten till. */
};

/******************************************************************************
* Strukten Button anv�nds f�r implementering av tryckknappar, som kan placeras 
* p� n�gon av digitala PINs 0 - 13 p� Arduino Uno. Det finns m�jlighet att
* enkelt l�sa av ifall tryckknappen �r nedtryckt. Det finns ocks� m�jlighet 
* att aktivera samt inaktivera PCI-avbrott p� tryckknappens PIN.
* 
* Avbrottsvektorer g�llande PCI-avbrott f�r respektive I/O-port �r f�ljande:
*
* I/O-port B (PIN 8 - 13): PCINT0_vect
* I/O-port C (PIN A0 - A5): PCINT1_vect - anv�nds dock inte f�r tryckknappar.
* I/O-port D (PIN 0 - 7): PCINT2_vect
******************************************************************************/
struct Button
{
	unsigned char PIN; /* Aktuellt PIN-nummer. */
	enum IO_port io_port; /* Anv�nd I/O-port. */
	enum bool interrupt_enabled; /* Indikerar ifall PCI-avbrott �r aktiverat. */
};

/* Funktionsdeklarationer: */
struct Led* new_Led(unsigned char* PIN); 
void Led_on(struct Led* self);
void Led_off(struct Led* self);
void Led_toggle(struct Led* self);
void Led_blink(struct Led* self, unsigned short* delay_time);
struct Button* new_Button(unsigned char* PIN); 
enum bool Button_is_pressed(struct Button* self);
void Button_enable_interrupt(struct Button* self);
void Button_disable_interrupt(struct Button* self);
void delay(unsigned short* delay_time);

#endif /* GPIO_H_ */