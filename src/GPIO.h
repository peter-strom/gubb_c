
#ifndef GPIO_H_
#define GPIO_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"

/******************************************************************************
 * Strukten Led används för implementering av lysdioder, som kan placeras på
 * någon av digitala PINs 0 - 13 på Arduino Uno. Varje lysdiod kan tändas,
 * släckas och togglas.
 ******************************************************************************/
typedef struct Led
{
	uint8_t PIN;	 /* Aktuellt PIN-nummer. */
	bool enabled;	 /* Indikerar ifall lysdioden är på eller inte. */
	IO_port io_port; /* I/O-port som lysdioden är ansluten till. */
} Led;

/******************************************************************************
 * Strukten Button används för implementering av tryckknappar, som kan placeras
 * på någon av digitala PINs 0 - 13 på Arduino Uno. Det finns möjlighet att
 * enkelt läsa av ifall tryckknappen är nedtryckt. Det finns också möjlighet
 * att aktivera samt inaktivera PCI-avbrott på tryckknappens PIN.
 *
 * Avbrottsvektorer gällande PCI-avbrott för respektive I/O-port är följande:
 *
 * I/O-port B (PIN 8 - 13): PCINT0_vect
 * I/O-port C (PIN A0 - A5): PCINT1_vect - används dock inte för tryckknappar.
 * I/O-port D (PIN 0 - 7): PCINT2_vect
 ******************************************************************************/
typedef struct Button
{
	uint8_t PIN;			/* Aktuellt PIN-nummer. */
	IO_port io_port;		/* Använd I/O-port. */
	bool interrupt_enabled; /* Indikerar ifall PCI-avbrott är aktiverat. */
} Button;

/* Funktionsdeklarationer: */
Led new_Led(uint8_t PIN);
void Led_on(Led *self);
void Led_off(Led *self);
void Led_toggle(Led *self);
void Led_blink(Led *self, uint16_t delay_time);

Button new_Button(uint8_t PIN);
bool Button_is_pressed(Button *self);
void Button_enable_interrupt(Button *self);
void Button_disable_interrupt(Button *self);

#endif /* GPIO_H_ */