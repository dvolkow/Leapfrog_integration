#ifndef LEAPFROG_MEMORY_H
#define LEAPFROG_MEMORY_H  1

#include "leapfrog_precision.h"
#include "leapfrog_types.h"

#ifdef LP_MPFR_CASE 

        #include <gmp.h>
        #include <mpfr.h>

        #define LP_T_INIT(l) mpfr_init2(l, LP_PRECISION_BITS_DEFAULT);

        #define LP_T_RELEASE(l) mpfr_clear(l)

        #define DDOT_A_INIT(a, i)  for (i = 0; i < LEAPFROG_MAX_DIM; ++i)  \
                        mpfr_init2(GET_DDOT_A(a, i), LP_PRECISION_BITS_DEFAULT)

        #define DDOT_A_RELEASE(a, i)  for (i = 0; i < LEAPFROG_MAX_DIM; ++i)  \
                        mpfr_clear(GET_DDOT_A(a, i))

#else 
        
        #define LP_T_INIT(a) 
        #define LP_T_RELEASE(a) 
        #define DDOT_A_INIT(a, i)
        #define DDOT_A_RELEASE(a, i)

#endif // LP_MPFR_CASE

#endif // LEAPFROG_MEMORY_H
