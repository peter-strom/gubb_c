/* Inkluderingsdirektiv: */
#include "Byte.h"

/******************************************************************************
* Funktionen new_Byte anv�nds f�r att deklarera samt initiera ett objekt av
* unionen Byte, som anv�nds f�r att l�sa och skriva till 8-bitars register,
* samtidigt som enstaka bitar enkelt kan ett- eller nollst�llas samt l�sas av.
* Ing�ende argument data utg�rs av objektets startv�rde, som tilldelas till
* unionens 8-bitars medlem data, vilket medf�r att de enskilda �tta bitarna
* uppdateras till detta v�rde p� bin�r form. F�rst deklareras ett nytt objekt 
* av unionen i fr�ga d�pt self. D�refter initieras instansvariablerna. 
* F�rst initieras samtliga bitar till startv�rdet 0 via anrop av funktionen
* new_Bits. D�refter tilldelas startv�rdet till instansvariabeln data via
* tilldelning av ing�ende argument med samma namn. Denna tilldelning medf�r
* att de enskilda bitarna tilldelas detta v�rde p� bin�r form. D�refter
* returneras objektet vid �terhoppet, nu redo att anv�ndas.
******************************************************************************/
union Byte new_Byte(unsigned char data)
{
	union Byte self;
	self.bits = new_Bits();
	self.data = data;
	return self;
}

/******************************************************************************
* Funktionen new_Bits utg�r initieringsrutin f�r objekt av strukten Bits,
* som utg�r enskilda bitar till ett 8-bitars register. F�rst skapas ett objekt
* av strukten i fr�ga, som d�ps till self, f�ljt av att samtliga bitar 
* tilldelas startv�rdet 0. D�refter returneras objekt, nu redo att anv�ndas.
******************************************************************************/
struct Bits new_Bits(void)
{
	struct Bits self;
	self.bit0 = 0;
	self.bit1 = 0;
	self.bit2 = 0;
	self.bit3 = 0;
	self.bit4 = 0;
	self.bit5 = 0;
	self.bit6 = 0;
	self.bit7 = 0;
	return self;
}