#include "mmp/Pointer.h"

int testNew(){

	char* buffer = new char[1024];

	Dummy* obj = new(buffer) Dummy(2);

	obj->~Dummy();
	obj = 0;

	delete[] buffer;

	return 0;
}