#ifndef POINTER_H
#define POINTER_H

#include "mmp_types.h"

//NEVER DELETE A PLACEMENT NEW OBJECT
//MANUALLY CALL DESTRUCTOR

template<bool, typename t = void*>
struct Typedef {
	
};

template<typename t>
struct Typedef<true, t>{
	typedef t TYPE;
};

#define TYPE(t)			Typedef<t> = void::TYPE;
#define NONVOID(t)		template <typename t> TYPE(t);

struct blah {
	int stuff;
};

class Dummy {

	blah* data;
	size_t size;

public:
	Dummy();

	Dummy(Dummy const& dumb);
	Dummy(Dummy&& dumb);

	~Dummy();

	Dummy(size_t newSize);

	//Set them to delete if you wanna get rid of them
	//Dummy& operator =(Dummy const& dumb) = delete;
	Dummy& operator =(Dummy const& dumb);

	//Set them to default if you want them to just be the default function they provide
	//Dummy& operator =(Dummy&& dumb) = default;
	Dummy& operator =(Dummy&& dumb);

	blah* operator->(){
		return data;
	}

	var operator new(size_t size){
		return ::operator new(size);
	}

	void operator delete(var del){
		::operator delete(del);
	}

	var operator new(size_t size, var buffer){
		//return ::operator new(size, buffer);
		return buffer;
	}

	void operator delete(var buffer, var del){
		::operator delete(del);
	}
};

var operator new(size_t size);
void operator delete(var del);
var operator new(size_t size, var buffer);
void operator delete(var buffer, var del);

var operator new[](size_t size);
void operator delete[](var del);
var operator new[](size_t size, var buffer);
void operator delete[](var buffer, var del);

#endif // !POINTER_H

#include "_inl/Pointer.inl"