#include "ADC.h" /* Inkluderar egen headerfil. */


static void ADC_read(TempSensor* self);
static void init_ADC(void);
static void store_temperature(TempSensor* self);

/******************************************************************************
* Funktionen new_TempSensor används för implementering av en temperatursensor 
* ansluten till någon av analoga pinnar A0 - A5 via ett objekt av strukten
* TempSensor. Ingående argument PIN utgör en pekare till aktuellt PIN-nummer. 
* Efteråt initieras objektets instansvariabler, följt av att det initierade 
* objektet returneras.
******************************************************************************/
TempSensor new_TempSensor(uint8_t PIN)
{
	TempSensor self;
	
	self.PIN = PIN;
	self.ADC_result = 0x00;
	self.temperature.rounded = 0x00;
	self.temperature_vector = new_Vector();
	self.vector_capacity = 0x0A;
	self.vector_next = 0x00;
	init_ADC();
	return self;
}

/******************************************************************************
* Funktionen print_temperature används för att läsa av rumstemperaturen och 
* skriva till vår PC. Först läses temperatursensorn av och resultatet lagras 
* i konstanten ADC_result. Därefter beräknas motsvarande analoga inspänning Uin 
* med följande formel:
*							Uin = Vcc * ADC_result / ADC_MAX,
*
* där ADC_result är resultatet från senaste AD-omvandling (0 - 1023).
*
* Därefter beräknas motsvarande temperatur i grader Celcius via följande formel:
*
*							temperatur = 100 * Uin - 50,
*
* där Uin är den beräknade analoga inspänningen (0 - 5 V).
*
* Sedan transmitteras den beräknade temperaturen till vår PC via anrop av
*  funktionen serial_print_integer, som möjliggör sammansättning av text och 
* heltal samt seriell överföring. Därmed transmitteras textstycktet 
* "Temperature: %d degrees", där %d är formatspecificerare för heltal och 
* ersätts med avläst rumstemperatur.
*
* Slutligen skrivs genomstnittliga temperaturen ut via Vector_average-funktionen.
******************************************************************************/
void print_temperature(TempSensor* self)
{
	float Uin;
	ADC_read(self);
	Uin = VCC * (float)self->ADC_result / ADC_MAX;
	self->temperature.rounded = (int)(100 * Uin - 50 + 0.5);
	store_temperature(self);
	serial_print_integer("Temperature: %d degrees Celcius - ", self->temperature.rounded);
	serial_print_integer("Average temperature: %d degrees Celcius \n", Vector_average(&self->temperature_vector));
	return;
}

 /******************************************************************************
* Funktionen ADC_read används för att läsa av temperatursensorn och returnera 
* resultatet. Först väljs analog kanal för avläsning, vilket i detta fall är 
* temperatorsensorns PIN A1 (PORTC1), samtidigt som AD-omvandlaren sätts till 
* att matas med intern matningsspänning. Därefter aktiveras AD-omvandlaren och 
* startas med lägsta möjliga frekvens (125 kHz) för högsta möjliga precision. 
* Därefter väntar vi till att AD-omvandlingen är slutförd, vilket signaleras 
* via AD-omvandlarens interrupt-flagga ADIF (ADC Interrupt Flag), som då blir
* ettställd. För att sedan återställa ADIF inför nästa AD-omvandlaren så 
* ettställs denna, följt av att avläst resultat returneras vid återhoppet.
 ******************************************************************************/

static void ADC_read(TempSensor* self)
{
	ADMUX = ((1 << REFS0)|self->PIN);
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION_COMPLETE;
	RESET_ADC_INTERRUPT_FLAG;
	self->ADC_result = ADC;
	return;
}

 /******************************************************************************
* Initierings rutin för AD-omvandling
 ******************************************************************************/
static void init_ADC(void)
{
	SELECT_ANALOG_CHANNEL;
	START_AD_CONVERSION;
	WAIT_FOR_AD_CONVERSION_COMPLETE;
	RESET_ADC_INTERRUPT_FLAG;
	return;
}

/**
 * @brief Stores current temperature in vector
 * 
 * @param self pointer to a structure of type TempSensor
 */
static void store_temperature(TempSensor* self)
{
	if(self->temperature_vector.elements < self->vector_capacity)
	{
		Vector_push(&self->temperature_vector, self->temperature.rounded);
	}
	else
	{
		Vector_set(&self->temperature_vector, self->vector_next, self->temperature.rounded);
	}
	if (++self->vector_next >= self->vector_capacity)
	{
		self->vector_next = 0x00;
	}
}