
#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define F_CPU 160000000UL /* S�tter klockfrekvensen till 16 MHz. */

/* Inkluderingsdirektiv: */
#include <avr/io.h> /* Bibliotek f�r implementering av I/O g�llande AVR-enheter. */
#include <avr/interrupt.h> /* Bibliotek f�r avbrott. */
#include <stdio.h> /* Bibliotek f�r implementering av I/O i C. */
#include <stdlib.h> /* C:s standardbibliotek. */
#include "Byte.h" /* Headerfil f�r inkludering av union f�r enkel bithantering. */

/******************************************************************************
* F�r att aktivera avbrott globalt s� ettst�lls I-flaggan (Interrupt Flag)
* i statusregistret SREG, vilket �stadkommes via assemblerinstruktionen SEI
* (Set Interrupt Flag). F�r att vid behov inaktivera avbrott globalt, vilket
* exempelvis �r n�dv�ndigt vid skrivning till EEPROM-minnet, s� nollst�lls
* I-biten via assemblerinstruktionen CLI (Clear Interrupt Flag).
******************************************************************************/
#define ENABLE_INTERRUPTS sei() /* Aktiverar avbrott globalt. */
#define DISABLE_INTERRUPTS cli() /* Inaktiverar avbrott globalt. */

/******************************************************************************
* I programmet anv�nds bitmakron f�r bitoperationer. Bitmakron var mycket 
* utbrett innan inline-funktioner togs i bruk med C99.
******************************************************************************/
#define SET_BIT(REGISTER, BIT) REGISTER |= (1 << BIT) /* Ettst�ller bit i specifikt register. */
#define CLEAR_BIT(REGISTER, BIT) REGISTER &= ~(1 << BIT) /* Nollst�ller bit i specifikt register. */
#define READ_BIT(REGISTER, BIT) (REGISTER &= (1 << BIT)) /* Avl�ser bit i enskilt register. */
#define ASSIGN(REGISTER, BIT) REGISTER = (1 << BIT) /* Tilldelar nytt v�rde, ettst�ller specifik bit */
#define SET(REGISTER) REGISTER = 0xFF /* Ettst�ller samtliga bitar i register. */
#define CLEAR(REGISTER) REGISTER = 0x00 /* Nollst�ller samtliga bitar i register. */

/* Typdefinitioner: */
enum bool { false, true }; /* Realiserar datatypen bool. */
enum IO_port { IO_PORTB, IO_PORTC, IO_PORTD }; /* Enumeration f�r I/O-portar. */
enum TimerSelection { TIMER0, TIMER1, TIMER2 }; /* Enumeration f�r timerkretsar. */
enum DataType { INT, DOUBLE, SIZE_T, CHAR }; /* Val av datatyp f�r vektor. */

#endif /* DEFINITIONS_H_ */