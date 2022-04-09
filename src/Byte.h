
#ifndef BYTE_H_
#define BYTE_H_

/******************************************************************************
* Strukten Bits anv�nds f�r att enkelt komma �t enskilda bitar i ett 8-bitars
* register. Via anv�ndning av unionen Byte kan anv�ndaren l�sa och skriva
* 8-bitars v�rden och sedan l�sa av enskilda bitar en efter en.
* Instansvariabler bit0 - bit 7 anv�nds f�r att lagra varje bit i ett givet
* register, d�r datatypen unsigned char anv�nds. Eftersom enbart en bit
* beh�ver allokeras per variabel, s� skrivs : 1 efter variabeldeklarationen.
* Bitarna deklareras i ordningen 0 - 7, d� minne allokeras som s� i C att
* minst signifikant bit placeras f�rst (t�nk f�rsta index i en array).
******************************************************************************/
struct Bits
{
	unsigned char bit0 : 1; /* Bit 0 - Minst signifikant bit. */
	unsigned char bit1 : 1; /* Bit 1. */
	unsigned char bit2 : 1; /* Bit 2. */
	unsigned char bit3 : 1; /* Bit 3. */
	unsigned char bit4 : 1; /* Bit 4. */
	unsigned char bit5 : 1; /* Bit 5. */
	unsigned char bit6 : 1; /* Bit 6. */
	unsigned char bit7 : 1; /* Bit 7 - Mest signifikant bit. */
};

/******************************************************************************
* Unionen Byte anv�nds f�r att l�sa och skriva till ett 8-bitars register,
* samtidigt som enstaka bitar enkelt kan ett- eller nollst�llas samt l�sas av.
* Eftersom medlemmarna allokerar samma minnesutrymme, s� kan ett v�rde skrivas
* till den ena, s� uppdateras den �ndra efter detta v�rde. I detta fall g�ller
* d�rmed att ett 8-bitars v�rde kan skrivas till medlemmen data, vilket medf�r
* att medlemmen bits, som utg�r enskilda bitar, uppdateras till detta v�rde
* p� bin�r form. Samtidigt kan en given bit ett- eller nollst�llas via de
* enskilda bitarna, vilket p�verkar det 8-bitars v�rdet lagrat i data.
******************************************************************************/
union Byte
{
	unsigned char data; /* Data (en byte) f�r 8-bitars register. */
	struct Bits bits; /* Separata bitar f�r 8-bitars register. */
};

/* Externa funktioner: */
union Byte new_Byte(unsigned char data);
struct Bits new_Bits(void);

#endif /* BYTE_H_ */