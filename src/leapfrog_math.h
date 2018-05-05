#ifndef LEAPFROG_MATH_H
#define LEAPFROG_MATH_H  1

#include "leapfrog_types.h"
#include "leapfrog_compile.h"
#include "leapfrog_precision.h"


#ifdef LP_DEFAULT_ROUNDING_CASE

        #define leapfrog_to_double(l) (double)(l)

        #define leapfrog_sum(a, b) ((a) + (b))
        #define leapfrog_inc(a, b) (*(a) += (b))

        #define leapfrog_sub(a, b) ((a) - (b))
        #define leapfrog_dec(a, b) (*(a) -= (b))

        #define leapfrog_mul(a, b) ((a) * (b))
        #define leapfrog_pmul(a, b) (*(a) *= (b))

        #define leapfrog_div(a, b) ((a) / (b))
        #define leapfrog_pdiv(a, b) (*(a) /= (b))

#else

        #ifdef LP_MPFR_CASE

                #include <gmp.h>
                #include <mpfr.h>
        #endif


        double leapfrog_to_double(leapfrog_t *lp);

        void leapfrog_sum(leapfrog_t *res, leapfrog_t *a, 
                                                leapfrog_t *b); 

        void leapfrog_sub(leapfrog_t *res, leapfrog_t *a, 
                                                leapfrog_t *b);
        void leapfrog_mul(leapfrog_t *res, leapfrog_t *a, 
                                                leapfrog_t *b);
        void leapfrog_mul_2(leapfrog_t *res, leapfrog_t *a); 
        void leapfrog_div(leapfrog_t *res, leapfrog_t *a, 
                                                leapfrog_t *b);
        void leapfrog_div_2(leapfrog_t *res, leapfrog_t *a); 
        void leapfrog_sqrt(leapfrog_t *res, leapfrog_t *a); 
        void leapfrog_abs(leapfrog_t *res, leapfrog_t *a);
        void leapfrog_neg(leapfrog_t *res, leapfrog_t *a); 

        int leapfrog_cmp(leapfrog_t *a, leapfrog_t *b);
        int leapfrog_cmp_double(leapfrog_t *a, const double b);
#endif


#endif // LEAPFROG_MATH_H

