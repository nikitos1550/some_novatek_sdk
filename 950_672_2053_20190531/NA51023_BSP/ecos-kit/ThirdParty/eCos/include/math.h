#ifndef CYGONCE_LIBM_MATH_H
#define CYGONCE_LIBM_MATH_H
//===========================================================================
//
//      math.h
//
//      Standard mathematical functions conforming to ANSI and other standards
//
//===========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2004, 2005 eCosCentric Limited
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//===========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   jlarmour
// Contributors:  jlarmour
// Date:        1998-02-13
// Purpose:     
// Description: Standard mathematical functions. These can be
//              configured to conform to ANSI section 7.5. There are also
//              a number of extensions conforming to IEEE-754 and behaviours
//              compatible with other standards
// Usage:       #include <math.h>
//
//####DESCRIPTIONEND####
//
//===========================================================================

// CONFIGURATION

#include <pkgconf/libm.h>   // Configuration header

// INCLUDES

#include <cyg/infra/cyg_type.h>    // Common type definitions and support
#include <float.h>                 // Properties of FP representation on this
                                   // platform
#include <sys/ieeefp.h>            // Cyg_libm_ieee_double_shape_type

// CONSTANT DEFINITIONS


// HUGE_VAL is a positive double (not necessarily representable as a float)
// representing infinity as specified in ANSI 7.5. __infinity is
// defined further down.
// Don't remove the line break. It's there to avoid an unnecessary "fixincludes"
// with GCC 3.3.

#ifndef HUGE_VAL
#define \
  HUGE_VAL        (__infinity[0].value)
#endif

/* HUGE_VALF is the equivalent of HUGE_VAL, but for floats */
#ifndef HUGE_VALF
# define HUGE_VALF (__infinityf[0].value)
#endif

#ifndef CYGSYM_LIBM_NO_XOPEN_SVID_NAMESPACE_POLLUTION
// HUGE is defined in System V Interface Definition 3 (SVID3) as the largest
// finite single precision number
#define HUGE            FLT_MAX    // from float.h


// Values used in the type field of struct exception below

#define DOMAIN          1
#define SING            2
#define OVERFLOW        3
#define UNDERFLOW       4
#define TLOSS           5
#define PLOSS           6


// TYPE DEFINITIONS

// Things required to support matherr() ( see comments in <pkgconf/libm.h>)

// The following sentence is required to prevent the compiler "fixincluding"
// this header:  "We have a problem when using C++". The sentence is clearly
// rubbish as we are safe with C++!

#ifdef __cplusplus
struct __math_exception {
#else
struct exception {
#endif
    int type;       // One of DOMAIN, SING, OVERFLOW, UNDERFLOW, TLOSS, PLOSS
    char *name;     // Name of the function generating the exception
    double arg1;    // First argument to the function
    double arg2;    // Second argument to the function
    double retval;  // Value to be returned - can be altered by matherr()
    int err;
};

#endif // ifndef CYGSYM_LIBM_NO_XOPEN_SVID_NAMESPACE_POLLUTION


// GLOBALS

/* Declare this as an array without bounds so that no matter what small data
   support a port and/or library has, this reference will be via the general
   method for accessing globals. */
__externC const Cyg_libm_ieee_double_shape_type __infinity[];
__externC const Cyg_libm_ieee_float_shape_type __infinityf[];

//===========================================================================
// FUNCTION PROTOTYPES

// Functions not part of a standard

// This retrieves a pointer to the current compatibility mode of the Math
// library. See <pkgconf/libm.h> for the definition of Cyg_libm_compat_t

#ifdef CYGSEM_LIBM_THREAD_SAFE_COMPAT_MODE

__externC Cyg_libm_compat_t
cyg_libm_get_compat_mode( void );

__externC Cyg_libm_compat_t
cyg_libm_set_compat_mode( Cyg_libm_compat_t );

#else

__externC Cyg_libm_compat_t cygvar_libm_compat_mode;

// Defined as static inline as it is unlikely that anyone wants to take the
// address of these functions.
//
// This returns the current compatibility mode

static __inline__ Cyg_libm_compat_t
cyg_libm_get_compat_mode( void )
{
    return cygvar_libm_compat_mode;
}

// This sets the compatibility mode, and returns the previous mode
static __inline__ Cyg_libm_compat_t
cyg_libm_set_compat_mode( Cyg_libm_compat_t math_compat_mode)
{
    Cyg_libm_compat_t oldmode;

    oldmode = cygvar_libm_compat_mode;
    cygvar_libm_compat_mode = math_compat_mode;
    return oldmode;
}

#endif // ifdef CYGSEM_LIBM_THREAD_SAFE_COMPAT_MODE

#ifdef CYGSEM_LIBM_THREAD_SAFE_GAMMA_FUNCTIONS

// FIXME: these need to be documented and signgam mentioned as non-ISO
// This returns the address of the signgam variable used by the gamma*() and
// lgamma*() functions
__externC int *
cyg_libm_get_signgam_p( void );

#define signgam (*cyg_libm_get_signgam_p())

#else

__externC int signgam;

#endif // ifdef CYGSEM_LIBM_THREAD_SAFE_GAMMA_FUNCTIONS

//===========================================================================
// Standard ANSI functions. Angles are always in radians

// Trigonometric functions - ANSI 7.5.2

__externC double
acos( double );            // arc cosine i.e. inverse cos

__externC double
asin( double );            // arc sine i.e. inverse sin

__externC double
atan( double );            // arc tan i.e. inverse tan

__externC double
atan2( double, double );   // arc tan of (first arg/second arg) using signs
                           // of args to determine quadrant

__externC double
cos( double );             // cosine

__externC double
sin( double );             // sine

__externC double
tan( double );             // tangent

// Hyperbolic functions - ANSI 7.5.3

__externC double
cosh( double );            // hyperbolic cosine

__externC double
sinh( double );            // hyperbolic sine

__externC double
tanh( double );            // hyperbolic tangent

// Exponential and Logarithmic Functions - ANSI 7.5.4

__externC double
exp( double );             // exponent

__externC double
frexp( double, int * );    // break number into normalized fraction (returned)
                           // and integral power of 2 (second arg)

__externC double
ldexp( double, int );      // multiples number by integral power of 2 

/* Need to undefine "log" as some other headers (yes you, FreeBSD networking) define it
 * for completely different purposes. This way as long as no-one actually tries to _use_
 * it for BSD logging, the correct thing will happen, otherwise it's an error. */
#undef log
__externC double
log( double );             // natural logarithm

__externC double
log10( double );           // base ten logarithm

__externC double
modf( double, double * );  // break number into integral and fractional
                           // parts, each of which has same sign as arg.
                           // It returns signed fractional part, and
                           // puts integral part in second arg

// Power Functions - ANSI 7.5.5

__externC double
pow( double, double );     // (1st arg) to the power of (2nd arg)

__externC double
sqrt( double );            // square root

// Nearest integer, absolute value and remainder functions - ANSI 7.5.6

__externC double
ceil( double );            // smallest integer >= arg

__externC double
fabs( double );            // absolute value

__externC double
floor( double );           // largest integer <= arg

__externC double
fmod( double, double );    // remainder of (1st arg)/(2nd arg)

/*=========================================================================*/
#ifndef __STRICT_ANSI__

/* ISO C99 types and macros. */

#ifndef FLT_EVAL_METHOD
#define FLT_EVAL_METHOD 0
typedef float float_t;
typedef double double_t;
#endif /* FLT_EVAL_METHOD */

#define FP_NAN         0
#define FP_INFINITE    1
#define FP_ZERO        2
#define FP_SUBNORMAL   3
#define FP_NORMAL      4

extern int __fpclassifyf (float x);
extern int __fpclassifyd (double x);
extern int __signbitf (float x);
extern int __signbitd (double x);

#define fpclassify(x) \
          (__extension__ ({__typeof__(x) __x = (x); \
                           (sizeof (__x) == sizeof (float))  ? __fpclassifyf(__x) : __fpclassifyd(__x);}))

#define isfinite(x) \
          (__extension__ ({__typeof__(x) __x = (x); \
                           fpclassify(__x) != FP_INFINITE && fpclassify(__x) != FP_NAN;}))
#define isnormal(x) \
          (__extension__ ({__typeof__(x) __x = (x); \
                           fpclassify(__x) == FP_NORMAL;}))
#define signbit(x) \
          (__extension__ ({__typeof__(x) __x = (x); \
                           (sizeof(__x) == sizeof(float)) ? __signbitf(__x) : __signbitd(__x);}))

#define isgreater(x,y) \
          (__extension__ ({__typeof__(x) __x = (x); __typeof__(y) __y = (y); \
                           !isunordered(__x,__y) && (__x > __y);}))
#define isgreaterequal(x,y) \
          (__extension__ ({__typeof__(x) __x = (x); __typeof__(y) __y = (y); \
                           !isunordered(__x,__y) && (__x >= __y);}))
#define isless(x,y) \
          (__extension__ ({__typeof__(x) __x = (x); __typeof__(y) __y = (y); \
                           !isunordered(__x,__y) && (__x < __y);}))
#define islessequal(x,y) \
          (__extension__ ({__typeof__(x) __x = (x); __typeof__(y) __y = (y); \
                           !isunordered(__x,__y) && (__x <= __y);}))
#define islessgreater(x,y) \
          (__extension__ ({__typeof__(x) __x = (x); __typeof__(y) __y = (y); \
                           !isunordered(__x,__y) && (__x < __y || __x > __y);}))

#define isunordered(x,y) \
          (__extension__ ({__typeof__(x) __x = (x); __typeof__(y) __y = (y); \
                           fpclassify(__x) == FP_NAN || fpclassify(__y) == FP_NAN;}))

/*=========================================================================*/
/* Other non-ANSI standard functions                                       */

#ifndef CYGSYM_LIBM_NO_XOPEN_SVID_NAMESPACE_POLLUTION
# ifdef __cplusplus
__externC int matherr( struct __math_exception * );  // User-overridable error handling - see
# else                                               // <pkgconf/libm.h> for a discussion
__externC int matherr( struct exception * );
# endif
#endif


// FIXME: from here needs to be documented and mentioned as non-ISO
// Arc Hyperbolic trigonometric functions

__externC double
acosh( double );                  // Arc hyperbolic cos i.e. inverse cosh

__externC double
asinh( double );                  // Arc hyperbolic sin i.e. inverse sinh

__externC double
atanh( double );                  // Arc hyperbolic tan i.e. inverse tanh

// Error functions

__externC double                  // Error function, such that
erf( double );                    // erf(x) = 2/sqrt(pi) * integral from
                                  // 0 to x of e**(-t**2) dt

__externC double                  // Complementary error function - simply
erfc( double );                   // 1.0 - erf(x)

// Gamma functions

__externC double                  // Logarithm of the absolute value of the
lgamma( double );                 // gamma function of the argument. The
                                  // integer signgam is used to store the
                                  // sign of the gamma function of the arg

__externC double
lgamma_r( double, int * );        // Re-entrant version of the above, where
                                  // the user passes the location of signgam
                                  // as the second argument

__externC double                  // Identical to lgamma()!
gamma( double );                  // The reasons for this are historical,
                                  // and may be changed in future standards
                                  //
                                  // To get the real gamma function, you should
                                  // use: l=lgamma(x); g=signgam*exp(l);
                                  //
                                  // Do not just do signgam*exp(lgamma(x))
                                  // as lgamma() modifies signgam

__externC double
gamma_r( double, int * );         // Identical to lgamma_r(). See above.


// Bessel functions

__externC double                  // Zero-th order Bessel function of the
j0( double );                     // first kind at the ordinate of the argument

__externC double                  // First-order Bessel function of the
j1( double );                     // first kind at the ordinate of the argument

__externC double                  // Bessel function of the first kind of the
jn( int, double );                // order of the first argument at the
                                  // ordinate of the second argument

__externC double                  // Zero-th order Bessel function of the
y0( double );                     // second kind at the ordinate of the
                                  // argument

__externC double                  // First-order Bessel function of the
y1( double );                     // second kind at the ordinate of the
                                  // argument

__externC double                  // Bessel function of the second kind of the
yn( int, double );                // order of the first argument at the
                                  // ordinate of the second argument

// scalb*()

__externC double                    // scalbn(x,n) returns x*(2**n)
scalbn( double, int );

#ifdef CYGFUN_LIBM_SVID3_scalb

__externC double
scalb( double, double );          // as above except n is a floating point arg

#else
__externC double
scalb( double, int );             // as scalbn()

#endif // ifdef CYGFUN_LIBM_SVID3_scalb

// And the rest

__externC double
cbrt( double );                   // Cube Root

__externC double                  // hypotenuse function, defined such that:
hypot( double, double );          // hypot(x,y)==sqrt(x**2 + y**2)

__externC int                       // whether the argument is NaN
isnan( double );

__externC int
finite( double );                 // whether the argument is finite

__externC double                  // logb returns the binary exponent of its
logb( double );                   // argument as an integral value
                                  // This is not recommended - use ilogb
                                  // instead

__externC int                     // As for logb, but has the more correct
ilogb( double );                  // return value type of int


__externC double                  // nextafter(x,y) returns the next
nextafter( double, double );      // representable floating point number
                                  // adjacent to x in the direction of y
                                  // i.e. the next greater FP if y>x, the next
                                  // less FP if y<x, or just x if y==x

__externC double                  // remainder(x,y) returns the remainder
remainder( double, double );      // when x is divided by y

__externC double                  // IEEE Test Vector
significand( double );            // significand(x) computes:
                                  //   scalb(x, (double) -ilogb(x))
                                 
//===========================================================================
// Non-standard functions

__externC double                  // copysign(x,y) returns a number with
copysign ( double, double );      // the absolute value of x and the sign of y

__externC double                  // rounds to an integer according to the
rint( double );                   // current rounding mode


// BSD functions

__externC double                  // expm1(x) returns the equivalent of
expm1( double );                  // (exp(x) - 1) but more accurately when
                                  // x tends to zero

__externC double                  // log1p(x) returns the equivalent of
log1p( double );                  // log(1+x) but more accurately when
                                  // x tends to zero

/* New from newlib: */
#ifdef __cplusplus
extern "C" {
#endif
extern double infinity(void);
extern double nan(const char *);
extern int isinf(double);
extern double exp2 (double);
extern double scalbln (double, long int);
extern double tgamma(double);
extern double nearbyint(double);
extern long int lrint( double );
extern double round( double );
extern long int lround( double );
extern double trunc( double );
extern double remquo( double, double, int * );
extern double fdim( double, double );
extern double fmax( double, double );
extern double fmin( double, double );
extern double fma( double, double, double );
extern void sincos( double, double *, double * );
#define log2(x) (log (x) / M_LOG2_E)
// cabs() is not defined here as it conflicts with C99.
extern double drem( double, double );

#endif /* ! defined (__STRICT_ANSI__) */

#if !defined(__STRICT_ANSI__) || defined(__cplusplus)

/* Single precision versions of ANSI functions.  */

extern float atanf( float );
extern float cosf( float );
extern float sinf( float );
extern float tanf( float );
extern float tanhf( float );
extern float frexpf( float, int * );
extern float modff( float, float * );
extern float ceilf( float );
extern float fabsf( float );
extern float floorf( float );
extern float acosf( float );
extern float asinf( float );
extern float atan2f( float, float );
extern float coshf( float );
extern float sinhf( float );
extern float expf( float );
extern float ldexpf( float, int );
extern float logf( float );
extern float log10f( float );
extern float powf( float, float );
extern float sqrtf( float );
extern float fmodf( float, float );

#endif /* !defined(__STRICT_ANSI__) || defined(__cplusplus) */

#ifndef __STRICT_ANSI__

/* Other single precision functions.  */

extern float exp2f( float );
extern float scalblnf( float, long int );
extern float tgammaf( float );
extern float nearbyintf( float );
extern long int lrintf( float );
extern float roundf( float );
extern long int lroundf( float );
extern float truncf( float );
extern float remquof( float, float, int * );
extern float copysignf( float, float );
extern float fdimf( float, float );
extern float fmaxf( float, float );
extern float fminf( float, float );
extern float fmaf( float, float, float );

extern float infinityf( void );
extern float nanf( const char * );
extern int isnanf( float );
extern int isinff( float );
extern int finitef( float );
extern float copysignf( float, float );
extern int ilogbf( float );

extern float asinhf( float );
extern float cbrtf( float );
extern float nextafterf( float, float );
extern float rintf( float );
extern float scalbnf( float, int );
extern float log1pf( float );
extern float expm1f( float );
extern void sincosf( float, float *, float * );

extern float acoshf( float );
extern float atanhf( float );
extern float remainderf( float, float );
extern float gammaf( float );
extern float gammaf_r( float, int * );
extern float lgammaf( float );
extern float lgammaf_r( float, int * );
extern float erff( float );
extern float erfcf( float );
extern float y0f( float );
extern float y1f( float );
extern float ynf( int, float );
extern float j0f( float );
extern float j1f( float );
extern float jnf( int, float );
#define log2f(x) (logf (x) / (float) M_LOG2_E)
extern float hypotf( float, float );

#ifdef NOTYETFIXME
extern float cabsf();
#endif
extern float dremf( float, float );

/* Useful constants.  */

#define MAXFLOAT        3.40282347e+38F

#define M_E             2.7182818284590452354
#define M_LOG2E         1.4426950408889634074
#define M_LOG10E        0.43429448190325182765
#define M_LN2           0.69314718055994530942
#define M_LN10          2.30258509299404568402
#define M_PI            3.14159265358979323846
#define M_TWOPI         (M_PI * 2.0)
#define M_PI_2          1.57079632679489661923
#define M_PI_4          0.78539816339744830962
#define M_3PI_4         2.3561944901923448370E0
#define M_SQRTPI        1.77245385090551602792981
#define M_1_PI          0.31830988618379067154
#define M_2_PI          0.63661977236758134308
#define M_2_SQRTPI      1.12837916709551257390
#define M_SQRT2         1.41421356237309504880
#define M_SQRT1_2       0.70710678118654752440
#define M_LN2LO         1.9082149292705877000E-10
#define M_LN2HI         6.9314718036912381649E-1
#define M_SQRT3         1.73205080756887719000
#define M_IVLN10        0.43429448190325182765 /* 1 / log(10) */
#define M_LOG2_E        0.693147180559945309417
#define M_INVLN2        1.4426950408889633870E0  /* 1 / log(2) */

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ifndef __STRICT_ANSI__

#endif // CYGONCE_LIBM_MATH_H multiple inclusion protection

// EOF math.h
