/* Inkluderingsdirektiv: */
#include "Serial.h"

 /******************************************************************************
 * Funktionen init_serial anv�nds f�r att initiera seriell �verf�ring. F�r att
 * inte genomf�ra multipla initieringar, s� unders�ks f�rst ifall den statiska
 * lokala variabeln serial_initialized �r true. Denna varibel �r satt till
 * statisk s� att minne enbart allokeras f�r denna en g�ng, d�r startv�rdet
 * s�tts till false. D�rmed skapas inte en ny lokal variant av denna variabel
 * varje g�ng funktionen exekverar. Ifall denna variabels v�rde senare �ndras 
 * till true, s� bibeh�lls detta v�rde. Varje g�ng funktionen exekverar s�
 * unders�ks d�rmed ifall denna variabels v�rde �r true.  Om detta �r fallet, 
 * vilket indikerar att seriell �verf�ring redan har initierats, s� avslutas 
 * funktionen direkt. Annars aktiveras seriell transmission f�r asynkron 
 * �verf�ring av en byte (motsvarar ett tecken) i taget med en bithastighet 
 * p� 115 220 kbps. F�r att f�rsta transmitterade utskrift skall hamna l�ngst
 * till v�nster p� den f�rsta raden s� transmitteras ett vagnreturstecken \r, 
 * f�ljt av ett nolltecken \0 f�r att indikera att transmissionen �r slutf�rd.
 ******************************************************************************/
void init_serial(void)
{
	static enum bool serial_initialized = false; 
	
	if (serial_initialized)
	{
		 return;
	}
	
	ENABLE_SERIAL_TRANSMISSION;
	SET_BAUD_RATE_TO_115220_KBPS;
	CARRIAGE_RETURN;
	END_TRANSMISSION;
	serial_initialized = true;
	return;
}

/******************************************************************************
* Funktionen serial_print anv�nds f�r att transmittera ett textstycke via
* seriell �verf�ring. Ing�ende argument s utg�r en pekare till textstycket.
* Varje tecken i str�ngen transmitteras en efter en tills ett nolltecken n�s,
* d�r funktionen write_byte anv�nds f�r att skicka respektive tecken. Ifall
* ett nyradstecken \n transmitteras s� trasmitteras ett vagnreturstecken \r
* direkt efter f�r att efterf�ljande tecken skall hamna l�ngst till v�nster
* p� n�sta rad. Transmissionen avslutas med att ett nolltecken \0 f�r att
* indikera textstyckets slut.
******************************************************************************/
void serial_print(char* s)
{
	unsigned long i = 0;
		
	loop:
		if (s[i])
		{
			write_byte(&s[i]);
			
			if (s[i++] == '\n')
			{
				CARRIAGE_RETURN;
			}
				
			goto loop;
		}
	
	END_TRANSMISSION;
	return;
}

/******************************************************************************
* Funktionen Serial_print_integer anv�nds f�r att sammans�tta ett textstycke
* med ett signerat heltal. Ing�ende argument s utg�r en pekare till textstycket,
* medan number utg�r det signerade talet. F�rst deklareras en str�ng d�pt text,
* som rymmer 100 tecken. Denna str�ng initieras till tom vid start genom att
* denna tilldelas ett nolltecken \0 l�ngst fram (p� index 0). Sedan sammans�tts
* inneh�llet som pekaren s pekar p� med det signerade heltalet number och
* lagras i str�ngen text, vilket �stadkommes via anrop av funktionen sprintf 
* fr�n biblioteket stdio.h. Detta sammansatta textstycke, som nu lagras i 
* str�ngen text, passeras sedan som ing�ende argument vid anrop av funktionen 
* serial_print f�r transmission.
******************************************************************************/
 void serial_print_integer(char* s, long* number)
{
	char text[SIZE];
	sprintf(s, text, *number);
	serial_print(s);
	return;
}

/******************************************************************************
* Funktionen serial_print_unsigned anv�nds f�r att sammans�tta ett textstycke
* med ett osignerat heltal. Ing�ende argument s utg�r en pekare till aktuellt
* textstycket, medan number utg�rs av det osignerade talet. F�rst deklareras 
* en str�ng d�pt text, som rymmer 100 tecken. D�refter sammans�tts inneh�llet 
* som pekaren s pekar p� med det osignerade heltalet number och lagras i 
* str�ngen text, vilket �stadkommes via anrop av funktionen sprintf fr�n 
* biblioteket stdio.h. Detta sammansatta textstycke, som nu lagras i str�ngen 
* text, passeras sedan som ing�ende argument vid anrop av funktionen 
* serial_print f�r transmission.
******************************************************************************/
void serial_print_unsigned(char* s, unsigned long* number)
{	
	char text[SIZE];
	sprintf(s, text, *number);
	serial_print(s);
	return;
}

/******************************************************************************
* Funktionen write_byte anv�nds f�r att transmittera en byte, vilket motsvarar
* ett tecken. Ing�ende argument data utg�rs av aktuellt tecken som skall
* transmitteras. F�rst v�ntar vi in att eventuellt f�reg�ende tecken har 
* transmitterats, vilket sker d� dataregistret UDR0 (USART Data Register 0).
* S� fort detta register �r tomt s� placeras det nya tecknet data i detta
* register f�r transmission. 
******************************************************************************/
void write_byte(char* data)
{
	WAIT_FOR_PREVIOUS_TRANSMISSION_TO_FINISH;
	UDR0 = *data;
	return;
}