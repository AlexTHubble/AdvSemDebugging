/*
Memory Management Playground (MMP)
By Daniel S. Buckstein
Copyright 2019-2020
*/
//-----------------------------------------------------------------------------

#include "mmp/mmp_memory.h"

#pragma comment(lib, "MemoryManagementPlayground.lib")


//-----------------------------------------------------------------------------

typedef		byte				chunk_kb[1024];


//-----------------------------------------------------------------------------

#define		decl_argc			ui32 const argc
#define		decl_argv			cstrp const argv[]
typedef		i32(*entry_func)(decl_argc, decl_argv);


//-----------------------------------------------------------------------------

int testMMP(decl_argc, decl_argv);
int testMalloc(decl_argc, decl_argv);


//-----------------------------------------------------------------------------

int main(decl_argc, decl_argv)
{
	//return testMMP(argc, argv);
	return testMalloc(argc, argv);
}


//-----------------------------------------------------------------------------

int testMMP(decl_argc, decl_argv)
{
	// stack-allocate a bunch of data
	chunk_kb chunk[12];
	size count = sizeof(chunk);
	ptr chunk_base = mmp_set_zero(chunk, count);



	// done, stack-allocated data popped
	return 0;
}


//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

int testMalloc(decl_argc, decl_argv)
{
	//var* p;
	//int* pi;
	//int const* kpi; //Cant change the value of the ptr (int const kpi[])
	//int* const pki; //Cant change the address of the ptr

	//int const* const kpki; //Cant change either value or address

	//union malloctest {
	//	i32 data[32];
	//	var pdata[32];
	//	struct {
	//		i32 dummy;
	//	};

	//};
	//typedef union malloctest malloctest;

	var base = HeapAlloc(GetProcessHeap(), 0, 1);
	var pool = mmp_pool_init(base, sizeof(int), 1024);
	var mallocTest = mmp_block_reserve(pool, 1024);

	if(mallocTest){
		printf("Works\n");
	}
	else {
		printf("Doesn't work\n");
	}

	mmp_block_release(mallocTest, pool);
	mmp_pool_term(pool);

	HeapFree(GetProcessHeap(), 0, base);

	//malloctest* test1024 = malloc(1024);
	//malloctest* test2048 = malloc(2048);
	//malloctest* test4098 = malloc(4098);

	//free(test1024);
	//
	//test1024 = malloc(8192);

	//free(test4098);
	//free(test1024);
	//free(test2048);

	// done
	return 0;
}
