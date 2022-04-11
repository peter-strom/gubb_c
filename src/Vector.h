#ifndef VECTOR_H_
#define VECTOR_H_

#include "definitions.h."
#include "Serial.h"

/******************************************************************************
* Strukten Vektor används för att lagra heltal av datatypen uint32_t i en
* dynamisk array. Arrayen allokeras på heapen, där medlemmen data pekar på
* första element, medan elements räknar antalet befintliga element (heltal).
*
* uint32_t* data; -Pekare till dynamiskt fält som lagrar osignerade heltal
* size_t elements; - håller reda på antal element i arrayen 
*
******************************************************************************/
typedef struct Vector
{
	uint32_t* data; 
	size_t elements;
	
} Vector;

// Externa funktioner;
Vector new_Vector(void);
void Vector_resize(Vector* self, const size_t new_size);
void Vector_push(Vector* self,const uint32_t new_element);
void Vector_clear(Vector* self);
void Vector_set(Vector* self, const size_t index, const uint32_t new_element);
uint32_t Vector_sum(const Vector* self);
double Vector_average(const Vector* self);
void Vector_print(const Vector* self);



#endif /* VECTOR_H_ */