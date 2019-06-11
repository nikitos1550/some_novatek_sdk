#ifndef _NVTTYPE_H
#define _NVTTYPE_H

#define FALSE 0
#define TRUE  1

typedef int                     BOOLEAN;
typedef unsigned long long      UINT64;
typedef long long               INT64;
typedef unsigned int            UINT32;
typedef long                    INT32;
typedef unsigned short          UINT16;
typedef short                   INT16;
typedef unsigned char           UINT8;
typedef char                    INT8;
typedef unsigned int            UINT;
typedef int                     INT;
typedef char                    CHAR;       ///< Character type (8 bits)
typedef unsigned int            BOOL;           ///< Boolean

/**
    @name Assert macros
*/
//@{
#define ASSERT_CONCAT_(a, b)    a##b
#define ASSERT_CONCAT(a, b)     ASSERT_CONCAT_(a, b)

#if defined(__COUNTER__)

#define STATIC_ASSERT(expr) \
enum { ASSERT_CONCAT(FAILED_STATIC_ASSERT_, __COUNTER__) = 1/(expr) }

#else

// This might cause compile error when writing STATIC_ASSERT at the same line
// in two (or more) files and one file include another one.
#define STATIC_ASSERT(expr) \
enum { ASSERT_CONCAT(FAILED_STATIC_ASSERT_, __LINE__) = 1/(expr) }

#endif


/**
    @name Align macros

    Align floor, align round, and align ceil

    @note base must be 2^n, where n = 1, 2, 3, ...
*/
//@{
#define ALIGN_FLOOR(value, base)  ((value) & ~((base)-1))                   ///< Align Floor
#define ALIGN_ROUND(value, base)  ALIGN_FLOOR((value) + ((base)/2), base)   ///< Align Round
#define ALIGN_CEIL(value, base)   ALIGN_FLOOR((value) + ((base)-1), base)   ///< Align Ceil
//@}

/**
    @name Align (round off)

    Round Off to 32, 16, 8 and 4

    @note Example: ALIGN_ROUND_32(32) = 32, ALIGN_ROUND_32(47) = 32, ALIGN_ROUND_32(48) = 64
*/
//@{
#define ALIGN_ROUND_32(a)       ALIGN_ROUND(a, 32)  ///< Round Off to 32
#define ALIGN_ROUND_16(a)       ALIGN_ROUND(a, 16)  ///< Round Off to 16
#define ALIGN_ROUND_8(a)        ALIGN_ROUND(a, 8)   ///< Round Off to 8
#define ALIGN_ROUND_4(a)        ALIGN_ROUND(a, 4)   ///< Round Off to 4
//@}

/**
    @name Align (round up)

    Round Up to 32, 16, 8 and 4.

    @note Example: ALIGN_CEIL_32(32) = 32, ALIGN_CEIL_32(33) = 64, ALIGN_CEIL_32(63) = 64
*/
//@{
#define ALIGN_CEIL_32(a)        ALIGN_CEIL(a, 32)   ///< Round Up to 32
#define ALIGN_CEIL_16(a)        ALIGN_CEIL(a, 16)   ///< Round Up to 16
#define ALIGN_CEIL_8(a)         ALIGN_CEIL(a, 8)    ///< Round Up to 8
#define ALIGN_CEIL_4(a)         ALIGN_CEIL(a, 4)    ///< Round Up to 4
//@}

/**
    @name Align (round down)

    Round Down to 32, 16, 8 and 4.

    @note Example: ALIGN_FLOOR_32(32) = 32, ALIGN_FLOOR_32(33) = 32, ALIGN_FLOOR_32(63) = 32
*/
//@{
#define ALIGN_FLOOR_32(a)       ALIGN_FLOOR(a, 32)  ///< Round down to 32
#define ALIGN_FLOOR_16(a)       ALIGN_FLOOR(a, 16)  ///< Round down to 16
#define ALIGN_FLOOR_8(a)        ALIGN_FLOOR(a, 8)   ///< Round down to 8
#define ALIGN_FLOOR_4(a)        ALIGN_FLOOR(a, 4)   ///< Round down to 4
//@}



#endif
