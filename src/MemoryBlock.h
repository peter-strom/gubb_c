#ifndef MEMORY_BLOCK_H_
#define MEMORY_BLOCK_H_

/* Inkluderingsdirektiv: */
#include "definitions.h"
#include "Serial.h"

/*************************************************************************************
* Unionen MemoryBlock anv�nds f�r att kunna v�lja datatyp f�r en pekare till ett
* minnesblock p� heapen, vars datatyp kan v�ljas mellan int, double size_t samt char,
* d�r samma minnesutrymme anv�nds oavsett datatyp.
*************************************************************************************/
union MemoryBlock
{
	int* integer; /* Pekare till f�lt f�r signerade heltal. */
	double* decimal; /* Pekare till f�lt f�r flyttal. */
	size_t* natural; /* Pekare till f�lt f�r osignerade heltal. */
	char* character; /* Pekare till f�lt med tecken. */
};

/* Externa funktioner: */
union MemoryBlock* new_MemoryBlock(void);
void MemoryBlock_delete(union MemoryBlock* self, enum DataType* dataType);
enum bool MemoryBlock_resize(union MemoryBlock* self, enum DataType* dataType, size_t* new_size);
void MemoryBlock_assign(union MemoryBlock* self, enum DataType* dataType, 
						size_t* index, void* data);
void MemoryBlock_set(union MemoryBlock* self, enum DataType* dataType, 
					 size_t* index, void* data);
void* MemoryBlock_at(union MemoryBlock* self, enum DataType* dataType, size_t* index);
void MemoryBlock_print(union MemoryBlock* self, enum DataType* dataType, size_t* size);
void MemoryBlock_insert(union MemoryBlock* self, enum DataType* dataType, size_t* index, 
						size_t* size, void* data);
void MemoryBlock_erase(union MemoryBlock* self, enum DataType* dataType, 
					   size_t* index, size_t* size);
void MemoryBlock_sort(union MemoryBlock* self, enum DataType* dataType, size_t* size);
void MemoryBlock_sort_reverse(union MemoryBlock* self, enum DataType* dataType, size_t* size);

#endif /* MEMORY_BLOCK_H_ */