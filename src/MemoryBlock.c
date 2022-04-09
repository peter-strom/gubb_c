/* Inkluderingsdirektiv: */
#include "MemoryBlock.h"

/*************************************************************************************
* Funktionen new_MemoryBlock anv�nds f�r att deklarera och initiera ett nytt objekt 
* av unionen MemoryBlock, som utg�rs av ett flertal pekare till ett dynamiskt f�lt, 
* d�r samma minnesutrymme anv�nds oavsett vilken datatyp som anv�nds. Via pekare 
* integer, decimal, natural samt character s� kan samma minnesutrymme anv�ndas f�r 
* att lagra signerade eller osignerade heltal, flyttal eller tecken av n�gon av 
* datatyper int, double, size_t eller char. Dock kan enbart en av pekarna anv�ndas 
* vid ett givet tillf�lle.
*
* F�rst allokeras minne till ett objekt av unionen MemoryBlock, som d�ps till self.
* Ifall minnesallokeringen misslyckas s� returneras NULL direkt. Annars initieras
* respektive pekare till att peka p� NULL, vilket betyder att de f�r tillf�llet inte 
* pekar p� n�gon specifik minnesadress. Slutligen returneras objektet, som nu �r 
* redo att anv�ndas.
*************************************************************************************/
union MemoryBlock* new_MemoryBlock(void)
{
	union MemoryBlock* self = (union MemoryBlock*)malloc(sizeof(union MemoryBlock));
	if (!self) return NULL;
	(*self).integer = NULL;
	(*self).decimal = NULL;
	(*self).natural = NULL;
	(*self).character = NULL;
	return self;
}

/*************************************************************************************
* Funktionen MemoryBlock_delete anv�nds f�r att frig�ra minne f�r ett objekt av
* unionen MemoryBlock, vilket i inneb�r deallokering av ett dynamiskt f�lt.
* Ing�ende argument dataType indikerar objektets aktuella datatyp, som avg�r vilken
* av objektets pekare som pekar p� aktuellt f�lt. Datatypen unders�ks, f�ljt av att
* aktuellt f�lt frig�rs via anrop av funktionen free. Slutligen s�tts objektets
* samtliga pekare till att peka p� NULL, vilket inneb�r att de inte pekar p� n�gon
* specifik minnesadress.
*************************************************************************************/
void MemoryBlock_delete(union MemoryBlock* self, enum DataType* dataType)
{
	if (*dataType == INT)
	{
		free((*self).integer);
	}
	
	else if (*dataType == DOUBLE)
	{
		free((*self).decimal);
	}
	
	else if (*dataType == SIZE_T)
	{
		free((*self).natural);
	}
	
	else if (*dataType == CHAR)
	{
		free((*self).character);
	}
	
	(*self).integer = NULL;
	(*self).decimal = NULL;
	(*self).natural = NULL;
	(*self).character = NULL;
	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_resize anv�nds f�r att �ndra storleken p� ett dynamiskt
* f�lt via omallokering, implementerat via ett objekt av unionen MemoryBlock.
* Ing�ende argument dataType indikerar objektets aktiva datatyp, d�r motsvarande
* pekare integer, decimal, natural eller character pekar p� aktuellt f�lt. Ing�ende
* argument new_size utg�rs av f�ltets nya storlek efter omallokeringen.
*
* Utefter aktiv datatyp s� sker omallokering av aktuellt f�lt via anrop av funktionen
* realloc. F�r att inte f�rlora adressen och d�rmed tillg�ngen till befintligt f�lt
* ifall omallokeringen misslyckas, s� skapas ett kopiaf�lt som en pekare d�pt copy
* pekar p�, d�r befintligt inneh�ll kopieras �ver. Vid lyckad minnesallokering s�
* s�tts motsvarande pekare integer, decimal, natural eller character till att peka
* p� det nya f�lt som pekaren copy pekar p�. Ifall minnesallokeringen lyckas s�
* returneras true. Vid misslyckas minnesallokering, vilket indikeras genom att
* pekaren copy inte pekar p� n�gon minnesadress, s� returneras i st�llet false.
*************************************************************************************/
enum bool MemoryBlock_resize(union MemoryBlock* self, enum DataType* dataType, size_t* new_size)
{
	if (*dataType == INT)
	{
		int* copy = (int*)realloc((*self).integer, sizeof(int) * *new_size);
		if (!copy) return false;
		(*self).integer = copy;
		return true;
	}

	else if (*dataType == DOUBLE)
	{
		double* copy = (double*)realloc((*self).decimal, sizeof(double) * *new_size);
		if (!copy) return false;
		(*self).decimal = copy;
		return true;
	}

	else if (*dataType == SIZE_T)
	{
		size_t* copy = (size_t*)realloc((*self).natural, sizeof(size_t) * *new_size);
		if (!copy) return false;
		(*self).natural = copy;
		return true;
	}

	else if (*dataType == CHAR)
	{
		char* copy = (char*)realloc((*self).character, sizeof(char) * *new_size);
		if (!copy) return false;
		(*self).character = copy;
		return true;
	}

	return false;
}

/*************************************************************************************
* Funktionen MemoryBlock_assign anv�nds f�r att tilldela ett v�rde p� ett specifikt
* index i ett dynamiskt f�lt, som en pekare tillh�rande ett objekt av unionen
* MemoryBlock pekar p�. Ing�ende argument dataType indikerar objekts aktiva datatyp,
* index indikerar p� vilket index som tilldelning skall genomf�ras och data utg�r
* en pekare till det inneh�ll som skall skrivas till specificerat index.
*
* Utefter aktiv datatyp s� tilldelas inneh�llet till befintligt f�lt, d�r voidpekaren
* data typomvandlas till en pekare av aktiverad datatyp, exempelvis (int*)(data) f�r
* att omvandla fr�n en voidpekare till en intpekare ifall aktuell datatyp �r int.
* D�refter genomf�rs en dereferens f�r att komma �t inneh�llet som data pekar p�.
* Till exempel, om aktuell datatyp �r int och voidpekaren data har omvandlats till
* en intpekare, s� sker dereferens genom att placera en asterisk framf�r data. D�rmed
* skrivs *(int*)(data) f�r att f�rst typomvandla data till en intpekare och sedan
* komma �t inneh�llet som data pekar p�, d�r asterisken framf�r typomvandlingen
* medf�r dereferensen.
*************************************************************************************/
void MemoryBlock_assign(union MemoryBlock* self, enum DataType* dataType, 
						size_t* index, void* data)
{
	if (*dataType == INT)
	{
		(*self).integer[*index] = *(int*)(data);
	}
	
	else if (*dataType == DOUBLE)
	{
		(*self).decimal[*index] = *(double*)(data);
	}
	
	else if (*dataType == SIZE_T)
	{
		(*self).natural[*index] = *(size_t*)(data);
	}
	
	else if (*dataType == CHAR)
	{
		(*self).character[*index] = *(char*)(data);
	}
	
	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_set anv�nds f�r att skriva �ver inneh�llet p� ett specifikt
* index i en dynamisk array, implementerat via ett objekt av unionen MemoryBlock.
* Ing�ende argument dataType indikerar f�ltets datatyp, index utg�r index till det
* elements som skall skrivas �ver och data utg�r en pekare till det element som
* skall skrivas till aktuellt index. Beroende p� aktuell datatyp s� skrivs aktuell
* data som pekaren data pekar p� till specificerat index. F�r att kunna l�gga till
* datan s� typomvandlas voidpekaren data f�rst till en pekare av aktuell datatyp,
* f�ljt av en dereferens f�r att komma �t inneh�llet som pekaren pekar p�. D�rmed
* genomf�rs exempelvis operationen *(int*)(data) f�r att typomvandla voidpekaren
* data till en intpekare och sedan komma �t inneh�llet som denna pekar p� ifall
* aktuell datatyp �r int. Samma princip anv�nds �ven f�r �vriga datatyper.
*************************************************************************************/
void MemoryBlock_set(union MemoryBlock* self, enum DataType* dataType, 
					 size_t* index, void* data)
{
	if (*dataType == INT)
	{
		(*self).integer[*index] = *(int*)(data);
	}
	
	else if (*dataType == DOUBLE)
	{
		(*self).decimal[*index] = *(double*)(data);
	}
	
	else if (*dataType == SIZE_T)
	{
		(*self).natural[*index] = *(size_t*)(data);
	}
	
	else if (*dataType == CHAR)
	{
		(*self).character[*index] = *(char*)(data);
	}
	
	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_at anv�nds f�r att l�sa inneh�llet p� ett givet index i
* ett dynamiskt f�lt, implementerat via ett objekt av unionen MemoryBlock.
* Utefter aktuell datatyp s� returneras adressen till aktuellt element, vilket
* medf�r att en pekare till elementet i fr�ga returneras.
*
* Eftersom f�ltets datatyp varierar s� passeras en voidpekare till aktuellt element,
* vilket medf�r att inneh�llet efter l�sning m�ste typomvandlas till korrekt datatyp,
* f�ljt av en dereferens. Som exempel, om datatypen p� ett f�lt d�pt data �r int och
* elementet p� index 5 skall h�mtas och lagras i en variabel d�pt number s� b�r
* f�ljande operation genomf�ras:
* int number = *(int*)(MemoryBlock_at(&data, INT, 5);
*************************************************************************************/
void* MemoryBlock_at(union MemoryBlock* self, enum DataType* dataType, size_t* index)
{
	if (dataType == INT)
	{
		return &(*self).integer[*index];
	}
	
	else if (*dataType == DOUBLE)
	{
		return &((*self).decimal[*index]);
	}
	
	else if (*dataType == SIZE_T)
	{
		return &((*self).natural[*index]);
	}
	
	else if (*dataType == CHAR)
	{
		return &((*self).character[*index]);
	}
	
	return NULL;
}

/*************************************************************************************
* Funktionen MemoryBlock_print anv�nds f�r att skriva ut inneh�llet i ett dynamiskt
* f�lt, implementerat via ett objekt av unionen MemoryBlock. Ing�ende argument
* dataType indikerar f�ltets datatyp, medan size indikerar dess storlek. Om listan
* �r tom avslutas funktionen direkt. Annars skrivs varje element ur f�ltet ut, d�r
* tecken skrivs ut p� en enda rad f�r att implementera en str�ng inneh�llande ett
* textstycke, medan tal skrivs ut p� var sin rad. Utefter aktuell datatyp anv�nds
* l�mplig formatspecificerare, d�r c% anv�nds f�r char, %d f�r int, %g f�r double
* och %zu f�r size_t. Utskrift sker via seriell �verf�ring, fr�mst via funktioner
* serial_print, serial_print_integer samt serial_print_unsigned. Innen utskrift
* p�b�rjas s� initieras seriell �verf�ring via anrop av funktionen init_serial.
*************************************************************************************/
void MemoryBlock_print(union MemoryBlock* self, enum DataType* dataType, size_t* size)
{
	if (!(*size)) return;
	
	init_serial();

	if (*dataType == CHAR)
	{
		register size_t i;
		
		for (i = 0; i < *size; i++)
		{
			unsigned long character = (unsigned long)((*self).character[i]);
			serial_print_unsigned("%c", &character);
		}
		
		if ((*self).character[*size - 1] != '\n')
		{
			serial_print("\n");
		}
		
		return;
	}

	serial_print("-----------------------------------------------------------------------\n");
	
	register size_t i;
	
	for (i = 0; i < *size; i++)
	{
		if (*dataType == INT)
		{
			long integer = (long)((*self).integer[i]);
			serial_print_integer("%d\n", &integer);
		}
		
		else if (*dataType == DOUBLE)
		{
			long rounded_number = (long)((*self).decimal[i] + 0.5);
			serial_print_integer("%g\n", &rounded_number);
		}
		
		else if (*dataType == SIZE_T)
		{
			unsigned long number = (unsigned long)((*self).natural[i]);
			serial_print_unsigned("%zu\n", &number);
		}
	}

	serial_print("-----------------------------------------------------------------------\n\n");
	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_insert anv�nds f�r att l�gga in ett element i en dynamisk
* array, d�r efterf�ljande element flyttas ett steg fram�t. Innan denna funktion
* anropas s� m�ste aktuellt f�lt ha omallokerats f�r att rymma ytterligare ett
* element, annars kommer inte samtliga element f� plats i f�ltet. Det dynamiska
* f�ltet �r implementerat via ett objekt av unionen MemoryBlock. Ing�ende argument
* dataType indikerar f�ltets datatyp, index indikerar p� vilket index det nya
* elementet skall placeras, size indikerar f�ltets storlek och data utg�r en
* pekare till det element som skall l�ggas till.
*
* Utefter aktuell datatyp s� flyttas befintliga element som skall placeras bakom
* det nya elementet ett steg fram�t, vilket realiseras via en for-sats. Slutligen
* placeras det nya elementet p� specificerat index. F�r att komma �t inneh�llet
* som voidpekaren data pekar p�, s� typomvandlas denna f�rst till en pekare av
* aktuell datatyp, f�ljt av en dereferens f�r att komma �t det inneh�ll som denna
* pekar p�. Som exempel, om aktuell datatyp �r int, s� typomvandlas f�rst data
* till en intpekare via en explicit typomvandling (int*)(data), f�ljt av en derefens
* via en asterisk framf�r denna typomvandling. D�rmed skrivs *(int*)(data) f�r att
* f�rst typomvandla till en pekare av aktuell datatyp, f�ljt av en dereferens f�r
* att komma �t inneh�llet och tilldela detta till specicerat index i f�ltet.
*************************************************************************************/
void MemoryBlock_insert(union MemoryBlock* self, enum DataType* dataType, 
						size_t* index, size_t* size, void* data)
{
	register size_t i;
	
	if (*dataType == INT)
	{	
		for (i = *size; i > *index; i--)
		{
			(*self).integer[i] = (*self).integer[i - 1];
		}
		
		(*self).integer[*index] = *(int*)(data);
	}

	else if (*dataType == DOUBLE)
	{	
		for (i = *size; i > *index; i--)
		{
			(*self).decimal[i] = (*self).decimal[i - 1];
		}
		
		(*self).decimal[*index] = *(double*)(data);
	}
	
	else if (*dataType == SIZE_T)
	{	
		for (i = *size; i > *index; i--)
		{
			(*self).natural[i] = (*self).natural[i - 1];
		}
		
		(*self).natural[*index] = *(size_t*)(data);
	}

	else if (*dataType == CHAR)
	{	
		for (i = *size; i > *index; i--)
		{
			(*self).character[i] = (*self).character[i - 1];
		}
		
		(*self).character[*index] = *(char*)(data);
	}

	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_erase anv�nds f�r att ta bort ett element i en dynamisk
* array, d�r efterf�ljande element flyttas ett steg bak�t och d�rigenom skriver
* �ver elementet som skall tas bort. Efter att denna funktion har exekverat s� b�r
* aktuellt f�lt omallokeras f�r att rymma ett element mindre, d� f�ltet annars
* rymmer ett element mer �n vad arrayen skall inneh�lla. Det dynamiska f�ltet �r
* implementerat via ett objekt av unionen MemoryBlock. Ing�ende argument dataType
* indikerar f�ltets datatyp, index indikerar p� vilket index det nya elementet
* skall placeras och size indikerar f�ltets storlek.
*
* Utefter aktuell datatyp s� flyttas samtliga efterf�ljande element ett steg bak�t,
* vilket realiseras via en for-sats. D�rmed skrivs aktuellt element �ver, samtidigt
* som det sista elementet i arrayen f�rekommer p� de tv� sista adresserna i arrayen,
* eftersom detta kopieras fr�n befintligt sista index till f�reg�ende index utan att
* skrivas �ver. D�rmed b�r f�ltet sedan omallokeras till att rymma ett element
* mindre via anrop av funktionen MemoryBlock_resize, d�r det sista elementet i
* arrayen raderas.
*************************************************************************************/
void MemoryBlock_erase(union MemoryBlock* self, enum DataType* dataType, 
					   size_t* index, size_t* size)
{
	register size_t i;
	
	if (*dataType == INT)
	{
		for (i = *index; i < *size - 1; i++)
		{
			(*self).integer[i] = (*self).integer[i + 1];
		}
	}

	else if (*dataType == DOUBLE)
	{
		for (i = *index; i < *size - 1; i++)
		{
			(*self).decimal[i] = (*self).decimal[i + 1];
		}
	}

	else if (*dataType == SIZE_T)
	{
		for (i = *index; i < *size - 1; i++)
		{
			(*self).natural[i] = (*self).natural[i + 1];
		}
	}

	else if (*dataType == CHAR)
	{
		for (i = *index; i < *size - 1; i++)
		{
			(*self).character[i] = (*self).character[i + 1];
		}
	}

	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_sort anv�nds f�r att sortera inneh�llet i ett dynamiskt f�lt
* i stigande ordning, d�r det minsta talet placeras f�rst och det st�rsta talet
* placeras sist. Ing�ende argument dataType indikerar f�ltets datatyp, medan size
* indikerar dess storlek. Iteration sker genom f�ltet via n�stlade for-satser.
* Den yttre iteratorn i anv�nds f�r att iterera fr�n f�rsta till n�st sista element
* i f�ltet. Via den inre iteratorn j s� j�mf�rs elementet p� index i mot samtliga
* efterf�ljande element, som n�s via iterator j. Ifall elementet p� index i �r
* st�rre �n elementet p� index j s� byts elementen mot varandra. Detta genomf�rs
* genom att inneh�llet p� index i tempor�rt lagras via en variabel d�pt temp.
* Sedan skrivs inneh�llet p� index j till index i, f�ljt av att inneh�llet i
* variabeln temp skrivs till index j. Via likartade j�mf�relser och byten mellan
* alla element s� sorteras f�ltet.
*************************************************************************************/
void MemoryBlock_sort(union MemoryBlock* self, enum DataType* dataType, size_t* size)
{
	register size_t i, j;
	
	if (*dataType == INT)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).integer[i]) > ((*self).integer[j]))
				{
					int temp = (*self).integer[i];
					(*self).integer[i] = (*self).integer[j];
					(*self).integer[j] = temp;
				}
			}
		}
	}

	else if (*dataType == DOUBLE)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).decimal[i]) > ((*self).decimal[j]))
				{
					double temp = (*self).decimal[i];
					(*self).decimal[i] = (*self).decimal[j];
					(*self).decimal[j] = temp;
				}
			}
		}
	}

	else if (*dataType == SIZE_T)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).natural[i]) > ((*self).natural[j]))
				{
					size_t temp = (*self).natural[i];
					(*self).natural[i] = (*self).natural[j];
					(*self).natural[j] = temp;
				}
			}
		}
	}

	else if (*dataType == CHAR)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).character[i]) > ((*self).character[j]))
				{
					char temp = (*self).character[i];
					(*self).character[i] = (*self).character[j];
					(*self).character[j] = temp;
				}
			}
		}
	}

	return;
}

/*************************************************************************************
* Funktionen MemoryBlock_sort_reverse anv�nds f�r att sortera inneh�llet i ett
* dynamiskt f�lt i fallande ordning, d�r det st�rsta talet placeras f�rst och det
* minsta talet placeras sist. Ing�ende argument dataType indikerar f�ltets datatyp,
* medan size indikerar dess storlek. Iteration sker genom f�ltet via implementering
* av n�stlade for-satser samt tv� iteratorer i och j. Den yttre iteratorn i anv�nds
* f�r att iterera fr�n f�rsta till n�st sista element i f�ltet. Via inre iterator j
* s� j�mf�rs elementet p� index i mot samtliga efterf�ljande element. Ifall elementet
* p� index i �r mindre �n elementet p� index j s� byts elementen mot varandra. Detta
* �stadkommes genom att inneh�llet p� index i tempor�rt lagras via en variabel d�pt
* temp. Sedan skrivs inneh�llet p� index j till index i, f�ljt av att inneh�llet
* lagrat av variabeln temp skrivs till index j. Via likartade j�mf�relser och byten
* mellan samtliga element s� sorteras f�ltet.
*************************************************************************************/
void MemoryBlock_sort_reverse(union MemoryBlock* self, enum DataType* dataType, size_t* size)
{
	register size_t i, j;
	
	if (*dataType == INT)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).integer[i]) < ((*self).integer[j]))
				{
					int temp = (*self).integer[i];
					(*self).integer[i] = (*self).integer[j];
					(*self).integer[j] = temp;
				}
			}
		}
	}

	else if (*dataType == DOUBLE)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).decimal[i]) < ((*self).decimal[j]))
				{
					double temp = (*self).decimal[i];
					(*self).decimal[i] = (*self).decimal[j];
					(*self).decimal[j] = temp;
				}
			}
		}
	}

	else if (*dataType == SIZE_T)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).natural[i]) < ((*self).natural[j]))
				{
					size_t temp = (*self).natural[i];
					(*self).natural[i] = (*self).natural[j];
					(*self).natural[j] = temp;
				}
			}
		}
	}

	else if (*dataType == CHAR)
	{
		for (i = 0; i < *size - 1; i++)
		{
			for (j = i + 1; j < *size; j++)
			{
				if (((*self).character[i]) < ((*self).character[j]))
				{
					char temp = (*self).character[i];
					(*self).character[i] = (*self).character[j];
					(*self).character[j] = temp;
				}
			}
		}
	}

	return;
}
