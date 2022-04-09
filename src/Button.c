/* Inkluderingsdirektiv: */
#include "GPIO.h"

/******************************************************************************
* Initieringsrutin f�r tryckknappar. * Ing�ende argument PIN utg�r aktuellt 
* PIN-nummer sett till Arduino Uno (PIN 0 - 13), som �r ekvivalent med f�ljande:
*
*******************************************************************************
* PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
*     0 - 7                     D         Samma som PIN p� Arduino Uno        *
*     8 - 13                    B            PIN p� Arduino Uno - 8           *
*******************************************************************************
*
* F�rst allokeras minne f�r ett nytt objekt av strukten Button d�pt self. 
* Om minnesallokeringen misslyckas s� returneras NULL direkt. Annars initieras 
* objektets instansvariabler. Specificerat PIN-nummer sparas, f�ljt av att denna 
* unders�ks f�r att ta reda p� vilken I/O-port som tryckknappaen �r ansluten 
* till, d�r 0 - 7 inneb�r att tryckknappen �r ansluten till samma PIN p� 
* I/O-port D. Annars om aktuellt PIN-nummer ligger mellan 8 - 13, s� �r 
* tryckknappen ansluten till PIN 0 - 5  p� I/O-port B. En intern 
* pullup-resistor p� tryckknappens PIN aktiveras via ettst�llning av 
* motsvarande bit i aktuellt PORT-register, vilket medf�r att tryckknappens
* insignal alltid �r h�g eller l�g (0 eller 1).
******************************************************************************/
struct Button* new_Button(unsigned char* PIN)
{
	struct Button* self = (struct Button*)malloc(sizeof(struct Button));
	if (!self)
	{
		return NULL;
	}
	
	(*self).PIN = *PIN;
	(*self).interrupt_enabled = false;
	
	if ((*self).PIN >= 0 && (*self).PIN <= 7)
	{
		(*self).io_port = IO_PORTD;
		SET_BIT(PORTD, (*self).PIN);
	}
	
	else if ((*self).PIN >= 8 && (*self).PIN <= 13)
	{
		(*self).io_port = IO_PORTB;
		SET_BIT(PORTB, (*self).PIN);
	}
	
	return self;
}

/******************************************************************************
* Funktionen Button_is_pressed anv�nds f�r att indikera ifall en given 
* tryckknapp �r nedtryckt. Om tryckknappen �r ansluten till I/O-port B, s�
* l�ses motsvarande PIN fr�n registret PINB och returneras. Annars om 
* tryckknappen �r ansluten till I/O-port B, s� l�ses motsvarande PIN fr�n 
* registret PIND och returneras. Annars vid fel s� returneras false.
******************************************************************************/
enum bool Button_is_pressed(struct Button* self)
{
	if ((*self).io_port == IO_PORTB)
	{
		return (READ_BIT(PINB, (*self).PIN));
	}
	
	else if ((*self).io_port == IO_PORTD)
	{
		return (READ_BIT(PIND, (*self).PIN));
	}
	
	return false;	
}

/******************************************************************************
* Funktionen Button_enable_interrupt anv�nds f�r att aktivera PCI-avbrott p�
* en given PIN som en tryckknapp �r ansluten till. F�rst aktiveras avbrott
* globalt via ettst�llning av bit I (Interrupt Flag) i statusregistret SREG.
* D�refter unders�ks anv�nd I/O-port. Om tryckknappen �r ansluten till
* I/O-port B, s� ettst�lls biten PCIE0 (PIN Change Interrupt Enable 0) i
* kontrollregistret PCICR (PIN Change Interrupt Control Register), f�ljt av
* att motsvarande PCI-avbrott aktiveras i maskregistret PCMSK0 (PIN Change
* Mask Register 0). Annars om tryckknappen �r ansluten till I/O-port D,
* s� ettst�lls biten PCIE2 (PIN Change Interrupt Enable 2) i kontrollregistret
* PCICR (PIN Change Interrupt Control Register), f�ljt av
* att motsvarande PCI-avbrott aktiveras i maskregistret PCMSK2 (PIN Change
* Mask Register 2). Slutligen s�tts instansvariabeln interrupt_enabled till
* true f�r att indikera att abrott nu �r aktiverat.
******************************************************************************/
void Button_enable_interrupt(struct Button* self)
{
	if ((*self).io_port == IO_PORTB)
	{
		union Byte byte = new_Byte(PCICR);
		byte.bits.bit0 = 1;
		ASSIGN(PCICR, byte.data);
		ASSIGN(PCMSK1, (*self).PIN);
	}
	
	else if ((*self).io_port == IO_PORTD)
	{
		union Byte byte = new_Byte(PCICR);
		byte.bits.bit2 = 1;
		ASSIGN(PCICR, byte.data);
		ASSIGN(PCMSK2, (*self).PIN);
	}
	
	(*self).interrupt_enabled = true;
	return;
}

/******************************************************************************
* Funktionen Button_disable_interrupt anv�nds f�r att inaktivera avbrott f�r
* en given PIN, d�r en tryckknapp �r ansluten. Detta �stadkommes via
* nollst�llning av motsvarande bit i n�gon av registren PCMSK0 (I/O-port B)
* eller PCMSK2 (I/O-port D).
******************************************************************************/
void Button_disable_interrupt(struct Button* self)
{
	if ((*self).io_port == IO_PORTB)
	{
		union Byte byte = new_Byte(PCMSK0);
		byte.bits.bit0 = 0;
		ASSIGN(PCMSK0, byte.data);
	}
	
	else if ((*self).io_port == IO_PORTD)
	{
		union Byte byte = new_Byte(PCMSK2);
		byte.bits.bit2 = 0;
		ASSIGN(PCMSK2, byte.data);
	}
	
	(*self).interrupt_enabled = false;
	return;
}
