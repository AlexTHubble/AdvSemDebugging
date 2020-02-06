/*
Memory Management Playground (MMP)
By Daniel S. Buckstein
Copyright 2019-2020
*/
//-----------------------------------------------------------------------------

#include "mmp/mmp_memory.h"

#include "mmp/mmp_file.h"

#include <Windows.h>


//-----------------------------------------------------------------------------

//Add structs here

//-----------------------------------------------------------------------------
// internal utilities

static inline cstrp mmp_get_cstr_end(cstrp cstr)
{
	uindex i = 0;
	while (*cstr != 0 && i++ < szcstr)
		++cstr;
	return cstr;
}


static inline byte* mmp_set_byte(byte* block, byte const* const end, byte const value)
{
	while (block < end)
		*(block++) = value;
	return block;
}

static inline dword* mmp_set_dword(dword* block, dword const* const end, dword const value)
{
	while (block < end)
		*(block++) = value;
	return block;
}

static inline cstrp mmp_set_cstr(cstr cstr, dword const value)
{
	return (cstrp)mmp_set_dword((dword*)(cstr), (dword*)(cstr + szcstr), value);
}


static inline byte* mmp_copy_byte(byte* block_dst, byte const* block_src, byte const* const end_dst)
{
	while (block_dst < end_dst)
		*(block_dst++) = *(block_src++);
	return block_dst;
}

static inline dword* mmp_copy_dword(dword* block_dst, dword const* block_src, dword const* const end_dst)
{
	while (block_dst < end_dst)
		*(block_dst++) = *(block_src++);
	return block_dst;
}

static inline cstrp mmp_copy_cstr(cstr cstr_dst, cstrp const cstr_src)
{
	return (cstrp)mmp_copy_dword((dword*)(cstr_dst), (dword*)(cstr_src), (dword*)(cstr_dst + szcstr));
}


static inline byte const* mmp_compare_byte(byte const* block_0, byte const* block_1, byte const* const end_0)
{
	while (block_0 < block_0)
		if (*(block_0) == *(block_1++))
			++block_0;
		else
			break;
	return block_0;
}

static inline dword const* mmp_compare_dword(dword const* block_0, dword const* block_1, dword const* const end_0)
{
	while (block_0 < end_0)
		if (*(block_0) == *(block_1++))
			++block_0;
		else
			break;
	return block_0;
}

static inline cstrp mmp_compare_cstr(cstrp const cstr_0, cstrp const cstr_1)
{
	return (cstrp)mmp_compare_dword((dword*)(cstr_0), (dword*)(cstr_1), (dword*)(cstr_0 + szcstr));
}


//-----------------------------------------------------------------------------
// general memory utilities

addr mmp_set(addr const block, size const size_bytes, byte const value)
{
	if (block && size_bytes > 0)
	{
		byte const value2dword[szdword] = { value, value, value, value };
		dword const value_as_dword = *((dword*)value2dword);
		dword* const base = (dword*)block;
		byte const* const basebp = (byte*)block;
		size const dword_ct = size_bytes / szdword;

		// write integers until the last integer can fit
		dword const* itr = mmp_set_dword(base, (dword_ct + base), value_as_dword);

		// write the remaining bytes
		byte const* itrbp = mmp_set_byte((byte*)itr, (size_bytes + basebp), value);

		// done
		return block;
	}
	return 0;
}


var mmp_copy(var const block_dst, kvar const block_src, size const size_bytes)
{
	if (block_dst && block_src && block_dst != block_src && size_bytes > 0)
	{
		dword* const base_dst = (dword*)block_dst;
		dword const* const base_src = (dword*)block_src;
		byte const* const basebp_dst = (byte*)block_dst;
		size const dword_ct = size_bytes / szdword;
		
		// copy and write integers until the last integer can fit
		dword* itr_dst = mmp_copy_dword(base_dst, base_src, (dword_ct + base_dst));

		// copy and write the remaining bytes
		byte* itrbp_dst = mmp_copy_byte((byte*)itr_dst, ((byte const*)(dword_ct + base_src)), (size_bytes + basebp_dst));
	
		// done
		return block_dst;
	}
	return 0;
}


size mmp_compare(kvar const block_0, kvar const block_1, size const size_bytes)
{
	if (block_0 && block_1 && block_0 != block_1 && size_bytes > 0)
	{
		dword const* const base_0 = (dword*)block_0;
		dword const* const base_1 = (dword*)block_1;
		byte const* const basebp_0 = (byte*)block_0;
		size dword_ct = size_bytes / szdword;

		// compare integers until the last integer can fit
		dword const* itr_0 = mmp_compare_dword(base_0, base_1, (dword_ct + base_0));

		// compare the remaining bytes
		byte const* itrbp_0 = mmp_compare_byte((byte*)itr_0, ((byte const*)(itr_0 - base_0 + base_1)), (size_bytes + basebp_0));

		// done
		return (itrbp_0 - basebp_0);
	}
	return 0;
}


//-----------------------------------------------------------------------------
// pool utilities

var mmp_pool_init(var const block_base, size_t const block_base_size, size_t const pool_size_bytes)
{
	/*
		So here's what's going down
		0. Check to see if the block of memory is free?
		1. Setup the ground of the pool, this will include any info and gaurd spots
			1a. Hook the size into the ground of the pool
		2. Init the end cap of the pool
		3. Return the first spot of memory in the pool? (or the first pointer)
	*/
	if (block_base && block_base_size && pool_size_bytes)
	{
		//Base of the block will hold our size
		int* size = (int*)block_base - 4; //Sets up size to be at the location of the base + 1 ???
		memcpy(size, &block_base_size, sizeof(block_base_size)); //Can we use that without explosion???

		int* byteSize = (int*)block_base - 5; //Sets up size to be at the location of the base + 1 ???
		memcpy(byteSize, &pool_size_bytes, sizeof(pool_size_bytes)); //Can we use that without explosion???

		return block_base;
	}
	return 0;
}


size mmp_pool_term(var const pool)
{
	if (pool)
	{
		//Reverse pool_init
	}
	return 0;
}


//-----------------------------------------------------------------------------
// block utilities

var mmp_block_reserve(var const pool, size_t const size)
{
	/*
		1. Find a free spot
		2. Allocate an array using the free spot?
		3. Profit (return true)
	*/
#ifdef _WIN32
	if (pool && size)
	{
		return VirtualAlloc(pool, size, MEM_COMMIT, PAGE_READWRITE);
	}
#endif // _WIN32

	return 0;
}


size mmp_block_release(var const block, var const pool)
{
#ifdef _WIN32
	if (block && pool)
	{
		VirtualFree(block, 0, MEM_RELEASE);
	}
#endif // _WIN32

	return 0;
}


//-----------------------------------------------------------------------------
