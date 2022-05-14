# Projekt 1 - Inbyggda System 1 - Ela21, YRGO

Handledare: Erik Pihl

Dynamiskt mätsystem med behållare


## Introduktion

Projektet byggde på scenariot att en äldre kollega som lärde sig programmera för över 30 år sedan nu åkt på semester och lämnat efter sig ett ofullständigt program kodat i ANSI C / C89. Programmet kompilerade utan varken fel eller varningar men utförde heller ingenting och behövde en noggrann felsökning.
Den fiktiva kollegan var inte så förtjust i funktionspekare, konstanter, lagringsklasser, typdefinitioner eller pass-by-value utan var mer av en ”pekarextrimist” med stor förkärlek till bitmakron. 
Programmet skulle köras på ett inbyggt system beståendes av ett Arduino Uno, en temperatursensor, knapp och lysdiod. Systemet hade till uppgift att överföra avläst temperatur via seriell överföring. Systemet skulle göras dynamiskt så att genomsnittstiden mellan knapptryckningar avgjorde hur ofta temperaturmätningen skulle ske.

## Resultat

Ett mycket lärorikt, roligt och mycket väl planerat projekt som gav många timmars felsökande där de svåraste felen att hitta var skrivningar till fel register. Fick ny bekantskap med bitmakron som jag kom så pass bra överens med att jag lät dom vara kvar. Men det roligaste var nog att skriva funktionen DynamicTimer_set_capacity() där testresultatet är bifogat som bild. Den bästa lärdomen jag fick av detta projektet var att structar blir mycket snyggare och enklare utan funktionspekare.

![alt text](https://github.com/peter-strom/gubb_c/blob/main/dynamic_timer_test-code.png?raw=true)

Fig.1 - Kod för att testa DynamicTimer_set_capacity(). Efter 13 knapptryckningar förminskades vektorns kapacitet och dom senaste 5 värdena följde med till den förminskade vektorn. 


![alt text](https://github.com/peter-strom/gubb_c/blob/main/dynamic_timer_test-result.png?raw=true)

Fig.2 - Resultatet efter testet av DynamicTimer_set_capacity(). 


![alt text](https://github.com/peter-strom/gubb_c/blob/main/average_temperature.png?raw=true)

Fig.3 - Resultatet efter implementering av "Average temperature".


Se changelog.md för detaljerad information angående ändringar.

