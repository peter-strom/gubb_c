/* Inkluderingsdirektiv: */
#include "header.h"

/******************************************************************************
* Funktionen main utg�r programmets start- och slutpunkt. Programmets globala
* variabler initieras via anrop av funktionen setup. En goto-sats anv�nds f�r
* att h�lla ig�ng programmet s� l�nge matningssp�nning tillf�rs. Resterande
* del av programmet �r avbrottsgenererat, s� hopp sker kontinuerligt till
* adressen d�pt loop.
******************************************************************************/
int main(void)
{	
	setup();
    loop:
		goto loop;
	return 0;
}

