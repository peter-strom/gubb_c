#ifndef DYNAMICTIMER_H_
#define DYNAMICTIMER_H_

// Inkluderingsdirektiv:
#include "definitions.h"
#include "Timer.h"
#include "Vector.h"
#include "Serial.h"

#define MAX_CAPACITY 100

/**
 * @brief DDynamicTimer combines the struct Vector and Timer into one dynamic timer.
 * 
 */
typedef struct DynamicTimer 
{
	Timer timer;
	Vector interrupt_vector;
	uint32_t interrupt_counter;
	size_t capacity;
	size_t next;
	bool initiated;
} DynamicTimer;

DynamicTimer new_DynamicTimer(const TimerSelection timerSelection, const size_t capacity);
void DynamicTimer_on(DynamicTimer* self);
void DynamicTimer_off(DynamicTimer* self);
void DynamicTimer_toggle(DynamicTimer* self);
void DynamicTimer_count(DynamicTimer* self);
bool DynamicTimer_elapsed(DynamicTimer* self);
void DynamicTimer_clear(DynamicTimer* self);
void DynamicTimer_update(DynamicTimer* self);
void DynamicTimer_set_capacity(DynamicTimer* self, const size_t new_capacity);
void DynamicTimer_print(const DynamicTimer* self);


#endif /*DYNAMICTIMER_H_*/