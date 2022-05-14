# Projekt 1 ELA21, YRGO
Handledare Erik Pihl

## Changelog


### Commits on Apr 9, 2022

    commit 68be374
        -Cloned original repo and merged it into a platform.io project

    commit 4c612e9
	    -gitignore

    commit e661d80
        main.c
            -goto loop; -> while(true)

    commit 3826dc6
        -encoding: ISO 8859-1 -> UTF-8

    commit 6a54f1b
        definitions.h 
            -#define F_CPU 16 MHz

    commit 6c7294a
        definitions.h
            #define READ_BIT(REGISTER, BIT)
                -REGISTER &= (1 << BIT)) -> (REGISTER & (1 << BIT)

    commit 843ae70
        Button.c
        GPIO.h
        MemoryBlock.c
        MemoryBlock.h
        Serial.c
        Timer.c
        Timer.h
        definitions.h
        setup.c
            -typedef enum 

### Commits on Apr 10, 2022

    commit 45dff88 - Serial_fix
        MemoryBlock.c
            -unsigned long -> uint32_t
            -long -> int32_t
        Serial.c
            -115220kbps -> 9600kbps
            -goto loop; -> for loop;
            -long -> int32_t
            -unsigned long -> uint32_t
            -cleaned up pointers
        Serial.h
            -fixing macros
            -long -> int32_t
            -unsigned long -> uint32_t
            -cleaned up pointers
    
    commit fda48ab - ADC fix
        ADC.c
            -remove struct
            -cleaned up pointers
            -removed malloc for TempSensor
            -added inti_ADC()
            -SELECT_ANALOG_CHANNEL; -> ADMUX = ((1 << REFS0)|self->PIN);
        ADC.h
            -fixing macros
            -added typedef to union and struct
            -changed types to *int*_t
            -cleaned up pointers
        header.h
            -remove struct from declaration
        interrupts.c
            -cleaned up pointers
        setup.c
            -cleaned up pointers

    commit 722892d - Button fix
        Button.c
            -remove struct
            -cleaned up pointers
            -removed malloc for Button
            -fixed weird PCICR bit set
            -fixed weird PCMSK* bit reset
        GPIO.h
            -added typedef to struct
            -changed types to *int*_t
            -cleaned up pointers
        header.h
            -remove struct from declaration
        interrupts.c
            -cleaned up pointers
        setup.c
            -cleaned up pointers
    
    commit f35e2ad - Led fix
        GPIO.h
            -added typedef to struct
            -changed types to *int*_t
            -cleaned up pointers
            -removed delay() from global declaration
        Led.c
            -declared delay() as static
            -remove struct
            -cleaned up pointers
            -removed malloc for Led
            -ASSIGN -> SET_BIT
            -added Led_blink()
            -added delay()
        definitions.h
            -added util/delay.h library
        header.h
            -remove struct from declaration
        interrupts.c
            -cleaned up pointers
        setup.c
            -cleaned up pointers

### Commits on Apr 11, 2022

    commit 6450eee - Timer fix
        Timer.c
            -remove struct
            -float* delay_time -> uint32_t delay_time
            -cleaned up pointers
            -removed malloc for Timer
            -added asm("SEI") to init_timer()
            -changed types to *int*_t
        Timer.h
            -prescaler 0 -> 1024
            -fixed macros
            -added typedef to struct
            -changed types to *int*_t
            -cleaned up pointers
            -cleaned up struct
        header.h
            -remove struct from declaration
        interrupts.c
            -cleaned up pointers
        setup.c
            -cleaned up pointers
            -removed variables
            -decreased timer1 to 60000 ms 

    commit f59fe46 - Button miss
        Button.c
            -(self->interrupt_enabled == IO_PORTD) -> (self->io_port == IO_PORTD)

    commit 6656d6d - added Vector struct

### Commits on Apr 20, 2022 

    commit 4bd1fcf & 4cce145 - added DynamicTimer struct


### Commits on Apr 26, 2022

    commit 39dda89 - added WDT (WatchDog Timer)

    commit e86a50d -proper indent and format with shift+alt+f

### Commits on May 14, 2022

    commit 3f07b97 
        Serial.c
            -removed END_TRANSMISSION; from init_serial() 
             and serial_print() to prevent ugly output in VSCode

    commit 1d3ce09 - Average temperature print
        ADC.c
            -added a vector with the capacity to store 10 temperature measurements
            -prints out the average temperature from the vector
        ADC.h
            -added the new members needed for the vector.
        
