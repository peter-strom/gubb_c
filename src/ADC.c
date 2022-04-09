#include "ADC.h" /* Inkluderar egen headerfil. */

/******************************************************************************
* Funktionen new_TempSensor anv�nds f�r implementering av en temperatursensor 
* ansluten till n�gon av analoga pinnar A0 - A5 via ett objekt av strukten
* TempSensor. Ing�ende argument PIN utg�r en pekare till aktuellt PIN-nummer. 
* F�rst allokeras minne f�r ett nytt objekt av strukten TempSensor d�pt self.
* Ifall minnesallokeringen misslyckas s� avslutas funktionen direkt. Annars
* initieras objektets instansvariabler, f�ljt av att det initierade objektet
* returneras.
******************************************************************************/
struct TempSensor* new_TempSensor(unsigned char* PIN)
{
	struct TempSensor* self = (struct TempSensor*)malloc(sizeof(struct TempSensor));
	
	if (!self)
	{
		 return NULL;
	}
	
	(*self).PIN = *PIN;
	(*self).ADC_result = 0x00;
	(*self).temperature.rounded = 0x00;
	return self;
}

/******************************************************************************
* Funktionen print_temperature anv�nds f�r att l�sa av rumstemperaturen och 
* skriva till v�r PC. F�rst l�ses temperatursensorn av och resultatet lagras 
* i konstanten ADC_result. D�refter ber�knas motsvarande analoga insp�nning Uin 
* med f�ljande formel:
*							Uin = Vcc * ADC_result / ADC_MAX,
*
* d�r ADC_result �r resultatet fr�n senaste AD-omvandling (0 - 1023).
*
* D�refter ber�knas motsvarande temperatur i grader Celcius via f�ljande formel:
*
*							temperatur = 100 * Uin - 50,
*
* d�r Uin �r den ber�knade analoga insp�nningen (0 - 5 V).
*
* Slutligen transmitteras den ber�knade temperaturen till v�r PC via anrop av
*  funktionen serial_print_integer, som m�jligg�r sammans�ttning av text och 
* heltal samt seriell �verf�ring. D�rmed transmitteras textstycktet 
* "Temperature: %d degrees", d�r %d �r formatspecificerare f�r heltal och 
* ers�tts med avl�st rumstemperatur.
******************************************************************************/
void print_temperature(struct TempSensor* self)
{
	float Uin;
	ADC_read(&(*self).ADC_result);
	Uin = VCC * ((*self).ADC_result / ADC_MAX);
	(*self).temperature.rounded = (int)(100 * Uin - 50 + 0.5);
	serial_print_integer("Temperature: %d degrees Celcius\n", &((*self).temperature.rounded));
	return;
}

 /******************************************************************************
* Funktionen ADC_read anv�nds f�r att l�sa av temperatursensorn och returnera 
* resultatet. F�rst v�ljs analog kanal f�r avl�sning, vilket i detta fall �r 
* temperatorsensorns PIN A1 (PORTC1), samtidigt som AD-omvandlaren s�tts till 
* att matas med intern matningssp�nning. D�refter aktiveras AD-omvandlaren och 
* startas med l�gsta m�jliga frekvens (125 kHz) f�r h�gsta m�jliga precision. 
* D�refter v�ntar vi till att AD-omvandlingen �r slutf�rd, vilket signaleras 
* via AD-omvandlarens interrupt-flagga ADIF (ADC Interrupt Flag), som d� blir
* ettst�lld. F�r att sedan �terst�lla ADIF inf�r n�sta AD-omvandlaren s� 
* ettst�lls denna, f�ljt av att avl�st resultat returneras vid �terhoppet.
 ******************************************************************************/
void ADC_read(unsigned short* ADC_result)
{
	SELECT_ANALOG_CHANNEL;
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION_COMPLETE;
	RESET_ADC_INTERRUPT_FLAG;
	*ADC_result = ADC;
	return;
}
