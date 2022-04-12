// Inkluderingsdirektiv
#include "DynamicTimer.h"

// Statiska funktioner
static inline size_t check_capacity(const size_t capacity);

DynamicTimer new_DynamicTimer(const TimerSelection timerSelection, const size_t capacity)
{
	DynamicTimer self;
	self.timer = new_Timer(timerSelection, 0x00);
	self.interrupt_vector = new_Vector();
	self.interrupt_counter = 0x00;
	self.capacity = check_capacity(capacity);
	self.next = 0x00;
	self.initiated = false;
	return self;
}
/******************************************************************************
* Funktionen DynamicTimer_on utgör en wrapper för funktionen Timer_on, som används för
* att sätta på 
******************************************************************************/

void DynamicTimer_on(DynamicTimer* self)
{
	Timer_on(&self->timer);
	return;
}

void DynamicTimer_off(DynamicTimer* self)
{
	
	Timer_off(&self->timer);
	return;
}
void DynamicTimer_toggle(DynamicTimer* self)
{
	Timer_toggle(&self->timer);
	return;
}
void DynamicTimer_count(DynamicTimer* self)
{
	if (self->timer.enabled)
	{
		self->timer.executed_interrupts++;
		self->interrupt_counter++;
	}	
	return;
}

bool DynamicTimer_elapsed(DynamicTimer* self)
{
	if (!self->timer.required_interrupts)
		return false;
	return Timer_elapsed(&self->timer);
}
void DynamicTimer_clear(DynamicTimer* self)
{
	Timer_off(&self->timer);
	Timer_clear(&self->timer);
	Vector_clear(&self->interrupt_vector);
	self->interrupt_counter = 0x00;
	self->next = 0x00;
	self->initiated = false;
	return;
}
void DynamicTimer_update(DynamicTimer* self)
{
	if (!self->initiated)
	{
		self->interrupt_counter = 0x00;
		self->initiated = true;
		serial_print("--------------------------------------------------------\n");
		serial_print("Dynamic timer initiated!\n");
		serial_print("--------------------------------------------------------\n");
		return;
	}
	if (self->interrupt_vector.elements < self->capacity)
	{
		Vector_push(&self->interrupt_vector, self->interrupt_counter);
	}
	else
	{
		Vector_set(&self->interrupt_vector, self->next, self->interrupt_counter);
	}
	
	self->interrupt_counter = 0x00;
	if (++self->next >= self->capacity) 
		{
			self->next = 0x00;	
		}
	//const uint32_t average_interrupts = Vector_average(&self->interrupt_vector);
	self->timer.required_interrupts = Vector_average(&self->interrupt_vector);
	serial_print("--------------------------------------------------------\n");
	serial_print("Dynamic timer updated!\n");
	DynamicTimer_print(self);
	
	/*if (self->interrupt_vector.elements == 5)
		{
			DynamicTimer_set_capacity(self, 20); // Test av set_capacity
		}*/
	
	
	return;
}
void DynamicTimer_set_capacity(DynamicTimer* self, const size_t new_capacity)
{
	if (!new_capacity) return;
	else if(new_capacity >= self->capacity)
	{
		serial_print_unsigned("Vector capacity resized from %lu ", self->capacity);
		self->capacity = new_capacity;
		serial_print_unsigned("to new value %lu\n\n", self->capacity);
		return;
	}
	
	const size_t last = self->interrupt_vector.elements -1;
	
	if (self->next >= self->interrupt_vector.elements/2)
	{
		for (register size_t i = 0; i < self->interrupt_vector.elements / 2; i++)
		{
			self->interrupt_vector.data[i] = self->interrupt_vector.data[last - i];
		}
	}
	
	serial_print_unsigned("Vector capacity resized from %lu ", self->capacity);
	Vector_resize(&self->interrupt_vector, new_capacity);
	self->capacity = new_capacity;
	serial_print_unsigned("to new value %lu\n\n", self->capacity);
	return;
}

/******************************************************************************
* Funktionen DynamicTimer_print används för att skriva ut information om en
* dynamisk timer, till exempel, aktuell fördröjningstid, antal lagrade element
* samt summan och genomsnittet av dessa.
******************************************************************************/
void DynamicTimer_print(const DynamicTimer* self)
{
	serial_print("--------------------------------------------------------\n");
	serial_print_unsigned("Vector capacity: %lu\n", self->capacity);
	serial_print_unsigned("Number of elements: %lu\n", self->interrupt_vector.elements);
	serial_print_unsigned("Next element in vector: %lu\n", self->next);
	serial_print_unsigned("Sum of stored elements: %lu\n", Vector_sum(&self->interrupt_vector));
	serial_print_unsigned("Average of values from vector: %lu\n", (uint32_t)(Vector_average(&self->interrupt_vector)+0.5));
	serial_print_unsigned("Delay time: %lu ms\n", self->timer.required_interrupts * INTERRUPT_TIME);
	serial_print("--------------------------------------------------------\n");
	
	return;
}

static inline size_t check_capacity(const size_t capacity)
{
	if (capacity > MAX_CAPACITY) 
		return MAX_CAPACITY;
	return capacity;
}