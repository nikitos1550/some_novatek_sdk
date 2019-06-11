#ifndef _PLATFORM_H
#define _PLATFORM_H

#if defined(__arm )
#define _ALIGNED(x) __align(x)
#define _PACKED_BEGIN __packed
#define _PACKED_END
#define _INLINE static __inline
#define _ASM_NOP __asm{nop;}
#define _SECTION(sec)
#define _CALLBACK()
#define _CALLBACK_SECTION(sec)
#define _MIPS_TODO 1
#define _DRV650_TODO 0
#define _STOPTRACE
#elif defined( __GNUC__)
#define _ALIGNED(x) __attribute__((aligned(x)))
#define _PACKED_BEGIN
#define _PACKED_END __attribute__ ((packed))
#define _INLINE static inline
#define _ASM_NOP __asm__("nop");
#define _SECTION(sec)          __attribute__ ((section (sec)))
#define _CALLBACK()            __attribute__ ((weak))
#define _CALLBACK_SECTION(sec) __attribute__ ((weak, section(sec)))
#define _MIPS_TODO 0
#define _DRV650_TODO 0
#define _STOPTRACE __attribute__((__no_instrument_function__))
#endif

#endif
