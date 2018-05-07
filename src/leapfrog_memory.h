/*
 * Then you use multiprecision lib-
 * rary with C, other you need mana-
 * gene memory. Below sample organi-
 * zing of it. Remember of default
 * types case!
 */
#ifndef LEAPFROG_MEMORY_H
#define LEAPFROG_MEMORY_H  1

#include "leapfrog_precision.h"
#include "leapfrog_types.h"

extern lp_param_t g_state;

#ifdef LP_MPFR_CASE 

        #include <gmp.h>
        #include <mpfr.h>

        #define LP_T_INIT(l) mpfr_init2((l), g_state.precision_bits)

        #define LP_T_RELEASE(l) mpfr_clear(l)

        #define DDOT_A_INIT(a, i)  for (i = 0; i < LEAPFROG_MAX_DIM; ++i)  \
                        mpfr_init2(GET_DDOT_A(a, i), g_state.precision_bits)

        #define DDOT_A_RELEASE(a, i)  for (i = 0; i < LEAPFROG_MAX_DIM; ++i)  \
                        mpfr_clear(GET_DDOT_A(a, i))

#else 
        /*
         * Default types case not
         * needed in memory managemnt
         */
        #define LP_T_INIT(a)   /* Nothing */
        #define LP_T_RELEASE(a) 
        #define DDOT_A_INIT(a, i)
        #define DDOT_A_RELEASE(a, i)

#endif // LP_MPFR_CASE

#endif // LEAPFROG_MEMORY_H
