// Inkluderingsdirektiv
#include "DynamicTimer.h"

// Statiska funktioner
static inline size_t check_capacity(const size_t capacity);

/**
 * @brief Initiates the structure TimerSelect
 *
 * @param timerSelection the desired timer.
 * @param capacity desired capacity of stored time messurements that
 * will result in an average delay time.
 * @return DynamicTimer
 */
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

/**
 * @brief turns the timer on
 *
 * @param self pointer to a structure of type DynamicTimer
 */
void DynamicTimer_on(DynamicTimer *self)
{
	Timer_on(&self->timer);
	return;
}

/**
 * @brief turns off the timer
 *
 * @param self pointer to a structure of type DynamicTimer
 */
void DynamicTimer_off(DynamicTimer *self)
{

	Timer_off(&self->timer);
	return;
}

/**
 * @brief toggles the timer on/off
 *
 * @param self pointer to a structure of type DynamicTimer
 */
void DynamicTimer_toggle(DynamicTimer *self)
{
	Timer_toggle(&self->timer);
	return;
}

/**
 * @brief counts interrupts.
 *
 * @param self pointer to a structure of type DynamicTimer
 */
void DynamicTimer_count(DynamicTimer *self)
{
	if (self->timer.enabled)
	{
		self->timer.executed_interrupts++;
		self->interrupt_counter++;
	}
	return;
}

/**
 * @brief checks if the timer has elapsed
 *
 * @param self pointer to a structure of type DynamicTimer
 * @return bool
 */
bool DynamicTimer_elapsed(DynamicTimer *self)
{
	if (!self->timer.required_interrupts)
	{
		return false;
	}
	return Timer_elapsed(&self->timer);
}

/**
 * @brief shuts down and resets the dynamic timer.
 *
 * @param self pointer to a structure of type DynamicTimer
 */
void DynamicTimer_clear(DynamicTimer *self)
{
	Timer_off(&self->timer);
	Timer_clear(&self->timer);
	Vector_clear(&self->interrupt_vector);
	self->interrupt_counter = 0x00;
	self->next = 0x00;
	self->initiated = false;
	return;
}

/**
 * @brief Initiates and updates the dynamic timer with the messured time since the last
 * timer reset.
 *
 * @param self pointer to a structure of type DynamicTimer
 */
void DynamicTimer_update(DynamicTimer *self)
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
		// test for DynamicTimer_set_capacity
		if (self->next == 3 && self->interrupt_vector.elements == 10)
		{
			DynamicTimer_set_capacity(self, 5);
			self->next = 0x05;
		}
		else
		{
			Vector_set(&self->interrupt_vector, self->next, self->interrupt_counter);
		}
	}

	self->interrupt_counter = 0x00;
	if (++self->next >= self->capacity)
	{
		self->next = 0x00;
	}

	self->timer.required_interrupts = Vector_average(&self->interrupt_vector);
	serial_print("--------------------------------------------------------\n");
	serial_print("Dynamic timer updated!\n");
	DynamicTimer_print(self);
	return;
}

/**
 * @brief Sets a new capacity for the dynamic timer.
 * And preserves the latest time messurments.
 * @param self pointer to a structure of type DynamicTimer
 * @param new_capacity desired capacity for number of stored time measurements.
 */
void DynamicTimer_set_capacity(DynamicTimer *self, const size_t new_capacity)
{
	if (!new_capacity)
		return;
	else if (new_capacity >= self->capacity)
	{
		serial_print_unsigned("Vector capacity resized from %lu ", self->capacity);
		self->capacity = new_capacity;
		serial_print_unsigned("to new value %lu\n\n", self->capacity);
		return;
	}

	// find the start index for the most recent data
	uint8_t latest_data = self->next - new_capacity;
	if (self->next < new_capacity)
	{
		latest_data = latest_data + self->capacity;
	}

	// copy the most recent data to a temporary array
	uint32_t temp_data[new_capacity];
	for (uint8_t i = 0; i < new_capacity; i++)
	{
		temp_data[i] = self->interrupt_vector.data[latest_data];
		if (++latest_data >= self->capacity)
		{
			latest_data = 0x00;
		}
	}

	serial_print_unsigned("Vector capacity resized from %lu ", self->capacity);
	Vector_resize(&self->interrupt_vector, new_capacity);
	self->capacity = new_capacity;
	self->interrupt_vector.data = temp_data;
	self->next = 0x00;
	serial_print_unsigned("to new value %lu\n\n", self->capacity);

	return;
}

/**
 * @brief Prints the current status of the dynamic timer.
 *
 * @param self pointer to a structure of type DynamicTimer
 */
void DynamicTimer_print(const DynamicTimer *self)
{
	serial_print("--------------------------------------------------------\n");
	serial_print_unsigned("Vector capacity: %lu\n", self->capacity);
	serial_print_unsigned("Number of elements: %lu\n", self->interrupt_vector.elements);
	serial_print_unsigned("Next element in vector: %lu\n", self->next);
	serial_print_unsigned("Sum of stored elements: %lu\n", Vector_sum(&self->interrupt_vector));
	serial_print_unsigned("Average of values from vector: %lu\n", (uint32_t)(Vector_average(&self->interrupt_vector) + 0.5));
	serial_print_unsigned("Delay time: %lu ms\n", self->timer.required_interrupts * INTERRUPT_TIME);
	for (uint8_t i = 0; i < self->interrupt_vector.elements; i++)
	{
		serial_print_unsigned("element: %d - ", i);
		serial_print_unsigned("value: %lu  \n", self->interrupt_vector.data[i]);
	}
	serial_print("--------------------------------------------------------\n");

	return;
}

/**
 * @brief Checks if the desired capacity exceeds the max limit.
 *
 * @param capacity desired capacity
 * @return size_t desired capacity or the maximum allowed capacity
 */
static inline size_t check_capacity(const size_t capacity)
{
	if (capacity > MAX_CAPACITY)
		return MAX_CAPACITY;
	return capacity;
}