/* Inkluderingsdirektiv: */
#include "header.h"

/******************************************************************************
* Avbrottsrutin f�r PCI-avbrott f�r I/O-port B. Vid aktivering av denna
* avbrottsrutin inaktiveras PCI-avbrott p� tryckknappens PIN 13, vilket i
* detta fall �r enda k�llan till PCI-avbrott p� I/O-porten i fr�ga. Detta g�rs
* f�r att f�rhindra p�verkan av kontaktstudsar, som annars kan medf�r att 
* multipla avbrott �ger rum kort efter varandra n�r knappen studsar. Timer 0
* aktiveras f�r att efter 300 ms �teraktivera PCI-avbrott p� PIN 13. Ifall
* nedtryckning av tryckknappen orsakade aktuellt avbrott, s� m�ts aktuell
* rumstemperatur och skrivs ut i en seriell terminal. F�r att indikera att
* temperaturavl�sning genomf�rs s� togglas led1. Samtidigt nollst�lls Timer 1.
******************************************************************************/
ISR (PCINT0_vect)
{
	Button_disable_interrupt(button);
	Timer_on(timer0);
	
	if (Button_is_pressed(button))
	{
		Timer_clear(timer1);
		print_temperature(tempSensor);		
		Led_toggle(led1);	
	}
	
	return;
}

/******************************************************************************
* Avbrottsrutin f�r Timer 0 i Normal Mode, vilket sker var 0.016:e millisekund
* d� timern i fr�ga �r aktiverad. Denna avbrottsrutin anv�nds f�r att generera 
* en bouncetid p� 300 ms, d�r PCI-avbrott p� PIN 13 h�lls inaktiverat efter ett 
* givet avbrott f�r att f�rhindra att multipla �ger rum p� grund av 
* kontakstudsar. N�r tillr�ckligt m�nga avbrott har �gt rum s� att timern har 
* l�pt ut, s� inaktiveras Timer 0 och PCI-avbrott p� PIN 13 �teraktiveras.
******************************************************************************/
ISR (TIMER0_OVF_vect)
{
	Timer_count(timer0);
	
	if (Timer_elapsed(timer0))
	{
		Timer_off(timer0);
		Button_enable_interrupt(button);	
	}
	
	return;
}

/******************************************************************************
* Avbrottsrutin f�r Timer 1 i CTC Mode, vilket sker var 0.016:e millisekund d�
* timern i fr�ga �r aktiverad. Denna avbrottsrutin anv�nds f�r att m�ta
* rumstemperaturen var 60:e sekund, alternativt 60 sekunder efter senaste
* knapptryckning. Varje g�ng denna rutin aktiveras s� r�knas antalet exekverade 
* avbrott upp. N�r tillr�ckligt m�nga avbrott har �gt rum s� att timern har l�pt 
* ut, s� m�ts rumstemperaturen och skrivs ut i terminalen. F�r att indikera att 
* temperaturm�tning genomf�rs s� togglas lysdioden. 
******************************************************************************/
ISR (TIMER1_COMPA_vect)
{
	Timer_count(timer1);	
	
	if (Timer_elapsed(timer1))
	{
		print_temperature(tempSensor);
		Led_toggle(led1);
	}
	return;
}