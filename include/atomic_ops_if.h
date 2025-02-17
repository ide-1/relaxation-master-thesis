/* this file was taken from libslock (https://github.com/tudordavid/libslock) */

/*
 * File: atomic_ops.h
 * Author: Tudor David <tudor.david@epfl.ch>
 *
 * Description:
 *      Cross-platform interface to common atomic operations
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Tudor David
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _ATOMIC_OPS_IF_H_INCLUDED_
#define _ATOMIC_OPS_IF_H_INCLUDED_

#include <inttypes.h>

//#  define CAS_BOOL(a,b,c) __sync_bool_compare_and_swap(a,b,c) //TODO Ivan
#ifdef __sparc__
	/*
	 *  sparc code
	 */
	#  include <atomic.h>

	//test-and-set uint8_t
	static inline uint8_t tas_uint8(volatile uint8_t *addr) {
	uint8_t oldval;
	  __asm__ __volatile__("ldstub %1,%0"
			: "=r"(oldval), "=m"(*addr)
			: "m"(*addr) : "memory");
		return oldval;
	}

	//Compare-and-swap
	#  define CAS_PTR(a,b,c) atomic_cas_ptr(a,b,c)
	#  define CAS_U8(a,b,c) atomic_cas_8(a,b,c)
	#  define CAS_U16(a,b,c) atomic_cas_16(a,b,c)
	#  define CAS_U32(a,b,c) atomic_cas_32(a,b,c)
	#  define CAS_U64(a,b,c) atomic_cas_64(a,b,c)
	//Swap
	#  define SWAP_PTR(a,b) atomic_swap_ptr(a,b)
	#  define SWAP_U8(a,b) atomic_swap_8(a,b)
	#  define SWAP_U16(a,b) atomic_swap_16(a,b)
	#  define SWAP_U32(a,b) atomic_swap_32(a,b)
	#  define SWAP_U64(a,b) atomic_swap_64(a,b)
	//Fetch-and-increment
	#  define FAI_U8(a) (atomic_inc_8_nv(a)-1)
	#  define FAI_U16(a) (atomic_inc_16_nv(a)-1)
	#  define FAI_U32(a) (atomic_inc_32_nv(a)-1)
	#  define FAI_U64(a) (atomic_inc_64_nv(a)-1)
	//Fetch-and-decrement
	#  define FAD_U8(a) (atomic_dec_8_nv(a,)+1)
	#  define FAD_U16(a) (atomic_dec_16_nv(a)+1)
	#  define FAD_U32(a) (atomic_dec_32_nv(a)+1)
	#  define FAD_U64(a) (atomic_dec_64_nv(a)+1)
	//Increment-and-fetch
	#  define IAF_U8(a) atomic_inc_8_nv(a)
	#  define IAF_U16(a) atomic_inc_16_nv(a)
	#  define IAF_U32(a) atomic_inc_32_nv(a)
	#  define IAF_U64(a) atomic_inc_64_nv(a)
	//Decrement-and-fetch
	#  define DAF_U8(a) atomic_dec_8_nv(a)
	#  define DAF_U16(a) atomic_dec_16_nv(a)
	#  define DAF_U32(a) atomic_dec_32_nv(a)
	#  define DAF_U64(a) atomic_dec_64_nv(a)
	//Test-and-set
	#  define TAS_U8(a) tas_uint8(a)
	//Memory barrier
	#  define MEM_BARRIER asm volatile("membar #LoadLoad | #LoadStore | #StoreLoad | #StoreStore");
	//end of sparc code

#elif defined(__tile__)
	/*
	 *  Tilera code
	 */
	#  include <arch/atomic.h>
	#  include <arch/cycle.h>
	//atomic operations interface
	//Compare-and-swap
	#  define CAS_PTR(a,b,c) arch_atomic_val_compare_and_exchange(a,b,c)
	#  define CAS_U8(a,b,c)  arch_atomic_val_compare_and_exchange(a,b,c)
	#  define CAS_U16(a,b,c) arch_atomic_val_compare_and_exchange(a,b,c)
	#  define CAS_U32(a,b,c) arch_atomic_val_compare_and_exchange(a,b,c)
	#  define CAS_U64(a,b,c) arch_atomic_val_compare_and_exchange(a,b,c)
	//Swap
	#  define SWAP_PTR(a,b) arch_atomic_exchange(a,b)
	#  define SWAP_U8(a,b) arch_atomic_exchange(a,b)
	#  define SWAP_U16(a,b) arch_atomic_exchange(a,b)
	#  define SWAP_U32(a,b) arch_atomic_exchange(a,b)
	#  define SWAP_U64(a,b) arch_atomic_exchange(a,b)
	//Fetch-and-increment
	#  define FAI_U8(a) arch_atomic_increment(a)
	#  define FAI_U16(a) arch_atomic_increment(a)
	#  define FAI_U32(a) arch_atomic_increment(a)
	#  define FAI_U64(a) arch_atomic_increment(a)
	//Fetch-and-decrement
	#  define FAD_U8(a) arch_atomic_decrement(a)
	#  define FAD_U16(a) arch_atomic_decrement(a)
	#  define FAD_U32(a) arch_atomic_decrement(a)
	#  define FAD_U64(a) arch_atomic_decrement(a)
	//Increment-and-fetch
	#  define IAF_U8(a) (arch_atomic_increment(a)+1)
	#  define IAF_U16(a) (arch_atomic_increment(a)+1)
	#  define IAF_U32(a) (arch_atomic_increment(a)+1)
	#  define IAF_U64(a) (arch_atomic_increment(a)+1)
	//Decrement-and-fetch
	#  define DAF_U8(a) (arch_atomic_decrement(a)-1)
	#  define DAF_U16(a) (arch_atomic_decrement(a)-1)
	#  define DAF_U32(a) (arch_atomic_decrement(a)-1)
	#  define DAF_U64(a) (arch_atomic_decrement(a)-1)
	//Test-and-set
	#  define TAS_U8(a) arch_atomic_val_compare_and_exchange(a,0,0xff)
	//Memory barrier
	#  define MEM_BARRIER arch_atomic_full_barrier()
	#  define LOAD_BARRIER arch_atomic_read_barrier()
	#  define STORE_BARRIER arch_atomic_write_barrier()

	static inline void
	AO_nop_full(void)
	{
	  MEM_BARRIER;
	}

	#  define AO_store_full(addr, val) arch_atomic_write(addr, val)
	#  define AO_load_full(addr)       arch_atomic_access_once((*addr))
	//Relax CPU
	//define PAUSE cycle_relax()

	//end of tilera code
#else
	/*
	 *  x86 code
	 */

	#if defined(__SSE__)
		#include <xmmintrin.h>
	#endif

	//Swap pointers
	static inline void* swap_pointer(volatile void* ptr, void *x)
	{
		#ifdef __i386__
			__asm__ __volatile__("xchgl %0,%1":"=r" ((unsigned) x):"m" (*(volatile unsigned *)ptr), "0" (x):"memory");
			return x;
		#elif defined(__x86_64__)
			__asm__ __volatile__("xchgq %0,%1":"=r" ((unsigned long long) x):"m" (*(volatile long long *)ptr), "0" ((unsigned long long) x):"memory");
			return x;
		#endif
	}

	//Swap uint64_t
	static inline uint64_t swap_uint64(volatile uint64_t* target,  uint64_t x)
	{
		__asm__ __volatile__("xchgq %0,%1":"=r" ((uint64_t) x):"m" (*(volatile uint64_t *)target), "0" ((uint64_t) x):"memory");
		return x;
	}

	//Swap uint32_t
	static inline uint32_t swap_uint32(volatile uint32_t* target,  uint32_t x)
	{
		__asm__ __volatile__("xchgl %0,%1":"=r" ((uint32_t) x):"m" (*(volatile uint32_t *)target), "0" ((uint32_t) x):"memory");
		return x;
	}

	//Swap uint16_t
	static inline uint16_t swap_uint16(volatile uint16_t* target,  uint16_t x)
	{
		__asm__ __volatile__("xchgw %0,%1":"=r" ((uint16_t) x):"m" (*(volatile uint16_t *)target), "0" ((uint16_t) x):"memory");
		return x;
	}

	//Swap uint8_t
	static inline uint8_t swap_uint8(volatile uint8_t* target,  uint8_t x)
	{
		__asm__ __volatile__("xchgb %0,%1":"=r" ((uint8_t) x):"m" (*(volatile uint8_t *)target), "0" ((uint8_t) x):"memory");
		return x;
	}

	//test-and-set uint8_t
	static inline uint8_t tas_uint8(volatile uint8_t *addr)
	{
		uint8_t oldval;
		__asm__ __volatile__("xchgb %0,%1": "=q"(oldval), "=m"(*addr): "0"((unsigned char) 0xff), "m"(*addr) : "memory");
		return (uint8_t) oldval;
	}
	//Taken from https://github.com/chaoran/fast-wait-free-queue/blob/master/primitives.h
	#define BTAS(ptr, bit) ({ \
  		char __ret; \
  		__asm__ __volatile__( \
      	"lock btsq %2, %0; setnc %1" \
      	: "+m" (*ptr), "=r" (__ret) : "ri" (bit) : "cc" ); \
  		__ret; \
	})

	//atomic operations interface
	//Compare-and-swap
	#  define CAS_PTR(a,b,c) __sync_val_compare_and_swap(a,b,c)
	#  define CAS_U8(a,b,c) __sync_val_compare_and_swap(a,b,c)
	#  define CAS_U16(a,b,c) __sync_val_compare_and_swap(a,b,c)
	#  define CAS_U32(a,b,c) __sync_val_compare_and_swap(a,b,c)
	#  define CAS_U64(a,b,c) __sync_val_compare_and_swap(a,b,c)

	/***************adon-*******************************************************/
	//bool __atomic_compare_exchange (type *ptr, type *expected, type *desired, bool weak, int success_memmodel, int failure_memmodel)

	#  define SWP(a, b)             __atomic_exchange_n(a, b, __ATOMIC_SEQ_CST)
	#  define CAE_16(a,b,c,d,e,f) 	__atomic_compare_exchange (a,b,c,d,e,f)
	#  define CAE(a,b,c) 			__atomic_compare_exchange (a,b,c,0,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST)
	#  define CAS(a,b,c) 			__sync_bool_compare_and_swap(a,b,c)
	#  define CAS_BOOL(a,b,c) 		__sync_bool_compare_and_swap(a,b,c)
	#  define CAS_16(a,b,c) 		__sync_bool_compare_and_swap_16(a,b,c)
	#  define FAXOR(a,b) 				__sync_fetch_and_or(a,b);
	/**********************************************************************/

	/******************** Kåre ******************************/
	// These commands are gcc builtins:
	// https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html

	#  define CAE_N(a,b,c,d,e,f) 	__atomic_compare_exchange_n (a,b,c,d,e,f)
	/********************************************************/

	//Swap
	#  define SWAP_PTR(a,b) swap_pointer(a,b)
	#  define SWAP_U8(a,b) swap_uint8(a,b)
	#  define SWAP_U16(a,b) swap_uint16(a,b)
	#  define SWAP_U32(a,b) swap_uint32(a,b)
	#  define SWAP_U64(a,b) swap_uint64(a,b)
	//Fetch-and-increment
	#  define FAI_U8(a) __sync_fetch_and_add(a,1)
	#  define FAI_U16(a) __sync_fetch_and_add(a,1)
	#  define FAI_U32(a) __sync_fetch_and_add(a,1)
	#  define FAI_U64(a) __sync_fetch_and_add(a,1)
	//Fetch-and-decrement
	#  define FAD_U8(a) __sync_fetch_and_sub(a,1)
	#  define FAD_U16(a) __sync_fetch_and_sub(a,1)
	#  define FAD_U32(a) __sync_fetch_and_sub(a,1)
	#  define FAD_U64(a) __sync_fetch_and_sub(a,1)
	//Increment-and-fetch
	#  define IAF_U8(a) __sync_add_and_fetch(a,1)
	#  define IAF_U16(a) __sync_add_and_fetch(a,1)
	#  define IAF_U32(a) __sync_add_and_fetch(a,1)
	#  define IAF_U64(a) __sync_add_and_fetch(a,1)
	//Decrement-and-fetch
	#  define DAF_U8(a) __sync_sub_and_fetch(a,1)
	#  define DAF_U16(a) __sync_sub_and_fetch(a,1)
	#  define DAF_U32(a) __sync_sub_and_fetch(a,1)
	#  define DAF_U64(a) __sync_sub_and_fetch(a,1)
	//Test-and-set
	#  define TAS_U8(a) tas_uint8(a)
	#  define TAS_U64(a,b) BTAS(a,b)
	//Memory barrier
	/* #define MEM_BARRIER __sync_synchronize() */
	#  define MEM_BARRIER //nop on the opteron for these benchmarks
	//Relax CPU
	//#define PAUSE _mm_pause()

	/*End of x86 code*/
#endif



/* start --generic code */

#define CAS_U64_bool(addr, old, new) (old == CAS_U64(addr, old, new))

/* static inline uint8_t */
/* CAS_U64_bool(volatile AO_t* addr, AO_t old, AO_t new) */
/* { */
/*   return (old == CAS_U64(addr, old, new)); */
/* } */



/* #define ATOMIC_CAS_MB(a, e, v)          (AO_compare_and_swap_full((volatile AO_t *)(a), (AO_t)(e), (AO_t)(v))) */
/* #define ATOMIC_FETCH_AND_INC_FULL(a)    (AO_fetch_and_add1_full((volatile AO_t *)(a))) */

#define ATOMIC_CAS_MB(a, e, v)          CAS_U64_bool((volatile AO_t *) (a),(AO_t) (e), (AO_t) (v))
#define ATOMIC_FETCH_AND_INC_FULL(a)    FAI_U32(a)

/* end -- generic code */



#endif



