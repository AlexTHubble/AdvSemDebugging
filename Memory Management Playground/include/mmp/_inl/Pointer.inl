
#ifdef POINTER_H
#ifndef POINTER_INL
#define POINTER_INL

template<typename t>
inline void* operator new(size_t size, t* buff){
	void* ptr = buff->malloc(size);
	return ptr;
}

template<typename t>
inline void operator delete(void* p, t* buff){
	buff->dealloc(p);
}

#endif //POINTER_INL
#endif //POINTER_H