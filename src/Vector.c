#include "Vector.h"

/******************************************************************************
 * Funktionen new_Vector används för att deklarera och initiera en ny dynamisk
 * array, implementerad via ett objekt av strukten Vector. Först skapas ett
 * nytt objekt av denna strukt, som döps till self, följt av att objektets
 * medlemmar initieras. Pekaren data sätts till en nullpekare, då denna inte
 * har någon befintlig minnesadress att peka på vid start (inget minne är
 * allokerat för arrayen). Instansvariabel elements sätts till 0x00 vid start,
 * då arrayen är tom. Slutligen returneras objektet, som nu är färdigt att
 * användas som en dynamisk array.
 ******************************************************************************/
Vector new_Vector(void)
{
	Vector self;
	self.data = NULL;
	self.elements = 0x00;
	return self;
}

/******************************************************************************
 * Funktionen Vector_resize används för att kunna uppdatera storleken på den
 * dynamiska vectorn. Om den andra parametern är 0 så kommer vectorn tas bort
 * via anrop på funktionen Vector_clear. Annars så reallokeras minne för till
 * en tillfällig kopia som får innehållet från den befintliga vectorn som till
 * sist skrivs tillbaka til
 ******************************************************************************/
void Vector_resize(Vector *self, const size_t new_size)
{
	if (!new_size)
	{
		Vector_clear(self);
		return;
	}
	uint32_t *copy = (uint32_t *)realloc(self->data, sizeof(uint32_t) * new_size);

	if (copy)
	{
		self->elements = new_size;
		self->data = copy;
	}
	return;
}

/******************************************************************************
 * Funktionen Vector_push används för att lägga till ett nytt element längst
 * bak i en array. Först skapas en kopia av befintligt fält, som döpt till
 * copy. Detta kopiafält rymmer ytterligare ett element än det gamla fältet.
 * Om minnesallokeringen misslyckades (brist på utrymme troligast) så returneras
 * NULL från funktionen realloc. Då avslutar vi funktionen, men det gamla
 * innehållet är kvar, då en kopia användes. Det nya elementet placeras längst
 * bak i det nya fältet (index elements) och antalet element räknas upp.
 * Datapekaren data sätts sedan till att peka på det nya fält som kopian
 * copy pekar på. Eftersom minnet för det gamla fältet frigjordes ifall
 * omallokeringen lyckades så krävs ingen deallokering.
 ******************************************************************************/
void Vector_push(Vector *self, const uint32_t new_element)
{
	uint32_t *copy = (uint32_t *)realloc(self->data, sizeof(uint32_t) * (self->elements + 1));
	if (copy)
	{
		copy[self->elements++] = new_element;
		self->data = copy;
	}
	return;
}

/******************************************************************************
 * Funktionen Vector_clear används för att tömma en dynamisk array och frigöra
 * minne. Deallokeringen sker via anrop av funktionen free, där vår datapekare
 * data, som pekar på det allokerade fältet, passeras. Pekaren data sätts
 * till nullpekare, så att denna inte pekar där det tidigare fältet låg.
 * Antalet element sätts till 0 för att indikera att arrayen är tom.
 ******************************************************************************/
void Vector_clear(Vector *self)
{
	free(self->data);
	self->data = NULL;
	self->elements = 0x00;
	return;
}

/******************************************************************************
 * Funktionen Vector_set används för att skriva över ett gammalt element.
 * Ingående argument index utgör index till det element som skall skrivas över,
 * medan new_element utgör det nya värdet. Om angivet index ligger utanför
 * arrayen så görs ingen skrivning. Annars läggs det nya elementet på angivet
 * index.
 ******************************************************************************/
void Vector_set(Vector *self, const size_t index, const uint32_t new_element)
{
	if (index < self->elements)
	{
		self->data[index] = new_element;
	}
	return;
}

/******************************************************************************
 * Funktionen Vector_sum används för att beräkna summan av samtliga befintliga
 * element- i en array. Om arrayen är tom så returneras summan 0. Annars sker
 * iteration genom arrayen från första till sista element, där varje lagrat
 * tal summeras via en lokal variabel sum, som returneras vid återhoppet.
 ******************************************************************************/
uint32_t Vector_sum(const Vector *self)
{
	uint32_t sum = 0x00;

	if (self->elements)
	{
		for (size_t i = 0; i < self->elements; i++)
		{
			sum += self->data[i];
		}
	}
	return sum;
}

/******************************************************************************
 * Funktionen Vector_average används för att beräkna medelvärdet av samtliga
 * lagrade tal. Om arrayen är tom returneras medelvärdet 0. Annars beräknas
 * summan av alla tal via anrop av funktionen Vector_sum. Medelvärdet beräknas
 * sedan som summan dividerat på antalet element, där flyttalsdivision används
 * för att erhålla ett korrekt värde (annars trunkeras eventuella decimaler).
 ******************************************************************************/
double Vector_average(const Vector *self)
{
	uint32_t sum = 0x00;

	if (self->elements)
	{
		sum = Vector_sum(self) / (double)self->elements;
	}
	return sum;
}

/******************************************************************************
 * Funktionen Vector_print används för att skriva ut befintligT innehåll i en
 * dynamisk array. Om arrayen är tom så avslutas funktionen direkt. Annars
 * initieras seriell överföring via anrop av funktionen init_serial. Sedan
 * skrivs antalet lagrade element i arrayen ut, följt av att varje element
 * skrivs ut var sin rad en efter en.
 ******************************************************************************/
void Vector_print(const Vector *self)
{
	if (self->elements)
	{
		init_serial();

		serial_print_unsigned("Number of elements: %lu\n", self->elements);
		serial_print_unsigned("Sum of all elements: %lu\n", Vector_sum(self));
		serial_print_integer("Rounded average: %ld\n", (int32_t)(Vector_average(self) + 0.5));
		serial_print("-------------------------------------------------------------------------------\n");
		for (register size_t i = 0; i < self->elements; i++)
		{
			serial_print_unsigned("%lu\n", self->data[i]);
		}
		serial_print("-------------------------------------------------------------------------------\n\n");
	}
	return;
}