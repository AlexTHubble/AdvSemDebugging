#include "mmp/Pointer.h"

Dummy::Dummy(){
	size = 0;
	data = 0;
}

Dummy::Dummy(Dummy const& dumb) : Dummy(dumb.size){
	if(data){

	}
}

Dummy::Dummy(Dummy&& dumb){
	data = dumb.data;
	size = dumb.size;
	dumb.data = 0;
}

Dummy::~Dummy(){
	delete[] data;
}

Dummy::Dummy(size_t newSize) : Dummy(){
	if (newSize > 0) {
		size = newSize;
		data = new blah[size];
	}
}

//If you wanna comment these out then you can set delete and default in header

Dummy& Dummy::operator=(Dummy const& dumb){
	if(data){
		delete[] data;
		size = 0;
	}
	if(dumb.data){
		size = dumb.size;
		data = new blah[size];
	}

	return *this;
}

Dummy& Dummy::operator=(Dummy&& dumb){
	if(data){
		size = 0;
		delete[] data;
	}

	size = dumb.size;
	data = dumb.data;
	dumb.data = 0;
	dumb.size = 0;

	return *this;
}


var operator new(size_t size){
	return 0;
}

void operator delete(var del){
	
}

var operator new(size_t size, var buffer){
	return 0;
}

var operator new[](size_t size){
	return 0;
}

void operator delete[](var del){

}

var operator new[](size_t size, var buffer){
	return 0;
}

void operator delete(var buffer, var del){

}

void operator delete[](var buffer, var del){

}