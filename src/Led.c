/* Inkluderingsdirektiv: */
#include "GPIO.h"
static void delay(uint16_t delay_time);

/******************************************************************************
 * Funktionen new_Led utgör initieringsrutin för objekt av strukten Led.
 * Ingående argument PIN utgör aktuellt PIN-nummer sett till Arduino Uno
 * (PIN 0 - 13), som är ekvivalent med följande:
 *
 *******************************************************************************
 * PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
 *     0 - 7                     D         Samma som PIN på Arduino Uno        *
 *     8 - 13                    B            PIN på Arduino Uno - 8           *
 *******************************************************************************
 *
 * Först deklareras ett nytt objekt av strukten Led döpt self. Om aktuellt
 * PIN-nummer ligger mellan 0 - 7, så är lysdioden ansluten till I/O-port D,
 * vilket lagras via instansvariabeln io_port. Aktuellt PORT-nummer är då samma
 * som PIN-numret, vilket lagras via instansvariabel PIN. Denna PIN sätts till
 * utport genom att motsvarande bit i datariktningsregister DDRD
 * (Data Direction Register D) ettställs. Bitvis OR |= används för att enbart
 * ettställa aktuell bit utan att påverka övriga bitar.
 *
 * Motsvarande genomförs ifall aktuellt PIN-nummer ligger mellan 8 - 13, med
 * skillnaden att I/O-porten då utgörs av I/O-port B, PIN-numret är lika med
 * erhållet PIN-nummer på Arduino Uno - 8, och motsvarande PIN sätts till
 * utport via ettställning av motsvarande bit i datariktningsregister DDRB
 * (Data Direction Register B).
 ******************************************************************************/
Led new_Led(uint8_t PIN)
{
	Led self;

	self.enabled = false;

	if ((PIN >= 0) && (PIN <= 7))
	{
		self.PIN = PIN;
		self.io_port = IO_PORTD;
		SET_BIT(DDRD, self.PIN);
	}

	else if ((PIN >= 8) && (PIN <= 13))
	{
		self.PIN = PIN - 8;
		self.io_port = IO_PORTB;
		SET_BIT(DDRB, self.PIN);
	}

	return self;
}

/******************************************************************************
 * Funktionen Led_on används för att tända en lysdiod. Ingående argument self
 * utgör en pekare till led-objektet i fråga. Utefter aktuell I/O-port så
 * ettställs motsvarande bit i register PORTB eller PORTD.
 ******************************************************************************/
void Led_on(Led *self)
{
	if (self->io_port == IO_PORTB)
	{
		SET_BIT(PORTB, self->PIN);
	}

	else if (self->io_port == IO_PORTD)
	{
		SET_BIT(PORTD, self->PIN);
	}

	self->enabled = true;
	return;
}

/******************************************************************************
 * Funktionen Led_off används för att släcka en lysdiod. Ingående argument
 * self utgör en pekare till lysdioden. Utefter aktuell I/O-port så nollställs
 * motsvarande bit i register PORTB eller PORTD.
 ******************************************************************************/
void Led_off(Led *self)
{
	if (self->io_port == IO_PORTB)
	{
		CLEAR_BIT(PORTB, self->PIN);
	}

	else if (self->io_port == IO_PORTD)
	{
		CLEAR_BIT(PORTD, self->PIN);
	}

	self->enabled = false;
	return;
}

/******************************************************************************
 * Funktionen Led_toggle används för att toggla en lysdiod. För att genomföra
 * detta undersöks medlemmen enabled. Om denna är true så är lysdioden tänd
 * och då släcks lysdioden via anrop av funktionen Led_off (via pekaren off).
 * Annars så tänds lysdioden via anrop av funktionen Led_on (via pekaren on).
 ******************************************************************************/
void Led_toggle(Led *self)
{
	if (self->enabled)
	{
		Led_off(self);
	}

	else
	{
		Led_on(self);
	}

	return;
}

void Led_blink(Led *self, uint16_t delay_time)
{
	Led_toggle(self);
	delay(delay_time);
	Led_toggle(self);
	delay(delay_time);
	return;
}

static void delay(uint16_t delay_time)
{
	for (uint16_t i = 0; i < delay_time; i++)
	{
		_delay_ms(1);
	}
	return;
}