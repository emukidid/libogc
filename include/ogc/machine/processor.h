#ifndef __OGC_MACHINE_PROCESSOR_H__
#define __OGC_MACHINE_PROCESSOR_H__

#include <gctypes.h>
#include "asm.h"

#define __stringify(rn)								#rn
#define ATTRIBUTE_ALIGN(v)							__attribute__((aligned(v)))
// courtesy of Marcan
#define STACK_ALIGN(type, name, cnt, alignment)		u8 _al__##name[((sizeof(type)*(cnt)) + (alignment) + (((sizeof(type)*(cnt))%(alignment)) > 0 ? ((alignment) - ((sizeof(type)*(cnt))%(alignment))) : 0))]; \
													type *name = (type*)(((u32)(_al__##name)) + ((alignment) - (((u32)(_al__##name))&((alignment)-1))))

#define _sync() __asm__ __volatile__("sync")
#define _isync() __asm__ __volatile__("isync")
#define _nop() __asm__ __volatile__("nop")
#define ppcsync() __asm__ __volatile__("sc")
#define ppchalt() ({					\
	__asm__ __volatile__("sync");		\
	while(1) {							\
		__asm__ __volatile__("nop");	\
		__asm__ __volatile__("li 3,0");	\
		__asm__ __volatile__("nop");	\
	}									\
})

#define mfpvr() ({register u32 _rval; \
		__asm__ __volatile__("mfpvr %0" : "=r"(_rval)); _rval;})

#define mfdcr(_rn) ({register u32 _rval; \
		__asm__ __volatile__("mfdcr %0," __stringify(_rn) : "=r" (_rval)); _rval;})
#define mtdcr(rn, val)  __asm__ __volatile__("mtdcr " __stringify(rn) ",%0" : : "r" (val))

#define mfmsr()   ({register u32 _rval; \
		__asm__ __volatile__("mfmsr %0" : "=r" (_rval)); _rval;})
#define mtmsr(val)  __asm__ __volatile__("mtmsr %0" : : "r" (val))

#define mfdec()   ({register u32 _rval; \
		__asm__ __volatile__("mfdec %0" : "=r" (_rval)); _rval;})
#define mtdec(_val)  __asm__ __volatile__("mtdec %0" : : "r" (_val))

#define mfspr(_rn) \
({	register u32 _rval = 0; \
	__asm__ __volatile__("mfspr %0," __stringify(_rn) \
	: "=r" (_rval));\
	_rval; \
})

#define mtspr(_rn, _val) __asm__ __volatile__("mtspr " __stringify(_rn) ",%0" : : "r" (_val))

#define mfwpar()		mfspr(WPAR)
#define mtwpar(_val)	mtspr(WPAR,_val)

#define mfmmcr0()		mfspr(MMCR0)
#define mtmmcr0(_val)	mtspr(MMCR0,_val)
#define mfmmcr1()		mfspr(MMCR1)
#define mtmmcr1(_val)	mtspr(MMCR1,_val)

#define mfpmc1()		mfspr(PMC1)
#define mtpmc1(_val)	mtspr(PMC1,_val)
#define mfpmc2()		mfspr(PMC2)
#define mtpmc2(_val)	mtspr(PMC2,_val)
#define mfpmc3()		mfspr(PMC3)
#define mtpmc3(_val)	mtspr(PMC3,_val)
#define mfpmc4()		mfspr(PMC4)
#define mtpmc4(_val)	mtspr(PMC4,_val)

#define mfhid0()		mfspr(HID0)
#define mthid0(_val)	mtspr(HID0,_val)
#define mfhid1()		mfspr(HID1)
#define mthid1(_val)	mtspr(HID1,_val)
#define mfhid2()		mfspr(HID2)
#define mthid2(_val)	mtspr(HID2,_val)
#define mfhid4()		mfspr(HID4)
#define mthid4(_val)	mtspr(HID4,_val)

#define mfthrm1()		mfspr(THRM1)
#define mtthrm1(_val)	mtspr(THRM1,_val)
#define mfthrm2()		mfspr(THRM2)
#define mtthrm2(_val)	mtspr(THRM2,_val)
#define mfthrm3()		mfspr(THRM3)
#define mtthrm3(_val)	mtspr(THRM3,_val)

#define __lhbrx(base,index)			\
({	register u16 res;				\
	__asm__ __volatile__ ("lhbrx	%0,%1,%2" : "=r"(res) : "b%"(index), "r"(base) : "memory"); \
	res; })

#define __lwbrx(base,index)			\
({	register u32 res;				\
	__asm__ __volatile__ ("lwbrx	%0,%1,%2" : "=r"(res) : "b%"(index), "r"(base) : "memory"); \
	res; })

#define __lswx(base,bytes)			\
({	register u32 res;				\
	__asm__ __volatile__ ("mtxer %2; lswx %0,%y1" : "=&b"(res) : "Z"(*(u32*)(base)), "r"(bytes) : "xer"); \
	res; })

#define __sthbrx(base,index,value)	\
	__asm__ __volatile__ ("sthbrx	%0,%1,%2" : : "r"(value), "b%"(index), "r"(base) : "memory")

#define __stwbrx(base,index,value)	\
	__asm__ __volatile__ ("stwbrx	%0,%1,%2" : : "r"(value), "b%"(index), "r"(base) : "memory")

#define __stswx(base,bytes,value)	\
	__asm__ __volatile__ ("mtxer %2; stswx %1,%y0" : "=Z"(*(u32*)(base)) : "r"(value), "r"(bytes) : "xer");

#define cntlzw(_val) ({register u32 _rval; \
					  __asm__ __volatile__("cntlzw %0, %1" : "=r"((_rval)) : "r"((_val))); _rval;})

#define _CPU_MSR_GET( _msr_value ) \
  do { \
    _msr_value = 0; \
    __asm__ __volatile__ ("mfmsr %0" : "=&r" ((_msr_value)) : "0" ((_msr_value))); \
  } while (0)

#define _CPU_MSR_SET( _msr_value ) \
{ __asm__ __volatile__ ("mtmsr %0" : "=&r" ((_msr_value)) : "0" ((_msr_value))); }

#define _CPU_ISR_Enable() \
	do { \
		register u32 _val = 0; \
		__asm__ __volatile__ ( \
			"mfmsr %0\n" \
			"ori %0,%0,0x8000\n" \
			"mtmsr %0" \
			: "=&r" (_val) : : "memory" \
		); \
	} while (0)

#define _CPU_ISR_Disable( _isr_cookie ) \
	do { \
		register u32 _disable_mask = 0; \
		__asm__ __volatile__ ( \
			"mfmsr %1\n" \
			"rlwinm %0,%1,0,17,15\n" \
			"mtmsr %0\n" \
			"extrwi %1,%1,1,16" \
			: "=&r" (_disable_mask), "=&r" (_isr_cookie) : : "memory" \
		); \
	} while (0)

#define _CPU_ISR_Restore( _isr_cookie )  \
	do { \
		register u32 _enable_mask = 0; \
		__asm__ __volatile__ ( \
			"mfmsr %0\n" \
			"insrwi %0,%1,1,16\n" \
			"mtmsr %0\n" \
			: "=&r" (_enable_mask) : "r" (_isr_cookie) : "memory" \
		); \
	} while (0)

#define _CPU_ISR_Flash( _isr_cookie ) \
	do { \
		register u32 _flash_mask = 0; \
		__asm__ __volatile__ ( \
			"mfmsr %0\n" \
			"insrwi %0,%1,1,16\n" \
			"mtmsr %0\n" \
			"mfmsr %0\n" \
			"rlwinm %0,%0,0,17,15\n" \
			"mtmsr %0\n" \
			: "=&r" (_flash_mask) : "r" (_isr_cookie) : "memory" \
		); \
	} while (0)

#define _CPU_FPR_Enable() \
{ register u32 _val = 0; \
	  __asm__ __volatile__ ("mfmsr %0; ori %0,%0,0x2000; mtmsr %0" : \
							"=&r" (_val) : "0" (_val));\
}

#define _CPU_FPR_Disable() \
{ register u32 _val = 0; \
	  __asm__ __volatile__ ("mfmsr %0; rlwinm %0,%0,0,19,17; mtmsr %0" : \
							"=&r" (_val) : "0" (_val));\
}

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

#ifndef bswap16
static inline u16 bswap16(u16 val)
{
	return __builtin_bswap16(val);
}
#endif

#ifndef bswap32
static inline u32 bswap32(u32 val)
{
	return __builtin_bswap32(val);
}
#endif

#ifndef bswap64
static inline u64 bswap64(u64 val)
{
	return __builtin_bswap64(val);
}
#endif

// Basic I/O

static inline u32 read32(u32 addr)
{
	u32 x;
	__asm__ __volatile__("lwz %0,0(%1) ; sync" : "=r"(x) : "b"(0xc0000000 | addr));
	return x;
}

static inline void write32(u32 addr, u32 x)
{
	__asm__("stw %0,0(%1) ; eieio" : : "r"(x), "b"(0xc0000000 | addr));
}

static inline void mask32(u32 addr, u32 clear, u32 set)
{
	write32(addr, (read32(addr)&(~clear)) | set);
}

static inline u16 read16(u32 addr)
{
	u16 x;
	__asm__ __volatile__("lhz %0,0(%1) ; sync" : "=r"(x) : "b"(0xc0000000 | addr));
	return x;
}

static inline void write16(u32 addr, u16 x)
{
	__asm__("sth %0,0(%1) ; eieio" : : "r"(x), "b"(0xc0000000 | addr));
}

static inline u8 read8(u32 addr)
{
	u8 x;
	__asm__ __volatile__("lbz %0,0(%1) ; sync" : "=r"(x) : "b"(0xc0000000 | addr));
	return x;
}

static inline void write8(u32 addr, u8 x)
{
	__asm__("stb %0,0(%1) ; eieio" : : "r"(x), "b"(0xc0000000 | addr));
}

static inline void writef32(u32 addr, f32 x)
{
	__asm__("stfs %0,0(%1) ; eieio" : : "f"(x), "b"(0xc0000000 | addr));
}

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif
