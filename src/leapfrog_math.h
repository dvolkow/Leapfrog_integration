/*
 * Math ops that abstract us from 
 * real types that used here. You
 * can use your type as 'leapfrog_t'
 * if to define operations below:
 */
#ifndef LEAPFROG_MATH_H
#define LEAPFROG_MATH_H  1

#include "leapfrog_types.h"
#include "leapfrog_compile.h"
#include "leapfrog_precision.h"


#ifndef LP_MPFR_CASE
        /*
         * Default you will have double precision
         * float point type for math operations.
         * Next step is using multiprecision lib-
         * raries for it but they are different as
         * interface. Interface that below is trying
         * make universal style for inline types and
         * libraries type.
         *   Case of inline types 'double' everyone
         * operations can be imagine with macrodefi-
         * nition.
         */
        #define leapfrog_to_double(l) (double)(l)

        #define leapfrog_sum(res, a, b)       (*(res) = *(a) + *(b))

        #define leapfrog_sub(res, a, b)       (*(res) = *(a) - *(b))

        #define leapfrog_mul(res, a, b)       (*(res) = *(a) * (*(b)))
        #define leapfrog_mul_d(res, a, b)     (*(res) = (*(a)) * (b))
        #define leapfrog_mul_2(res, a)        (*(res) = (*(a)) * 2)

        #define leapfrog_div(res, a, b)       (*(res) = (*(a)) / (*(b)))
        #define leapfrog_d_div(res, a, b)     (*(res) = (a) / (*(b)))
        #define leapfrog_div_2(res, a)        (*(res) = (*(a)) / 2)

        #define leapfrog_neg(res, a)          (*(res) = -(*(a)))
        #define leapfrog_abs(res, a)          (*(res) = fabs(*(a)))
        #define leapfrog_sqrt(res, a)         (*(res) = sqrt(*(a)))

        #define leapfrog_cmp(a, b)            (*(a) > *(b) ? 1 : -1)
        #define leapfrog_cmp_double(a, b)     (*(a) > (b) ? 1 : -1)

#else

        #include <gmp.h>
        #include <mpfr.h>

        /* Select rounding type from MPFR */
        #define LP_RND  MPFR_RNDD

        double leapfrog_to_double(leapfrog_t *lp);

        void leapfrog_sum(leapfrog_t *res, leapfrog_t *a, 
                                               const leapfrog_t *b); 

        void leapfrog_sub(leapfrog_t *res, leapfrog_t *a, 
                                                leapfrog_t *b);
        void leapfrog_mul(leapfrog_t *res, leapfrog_t *a, 
                                                leapfrog_t *b);
        void leapfrog_mul_d(leapfrog_t *res, leapfrog_t *a, 
                                                double b);
        void leapfrog_mul_2(leapfrog_t *res, leapfrog_t *a); 
        void leapfrog_div(leapfrog_t *res, leapfrog_t *a, 
                                                leapfrog_t *b);
        void leapfrog_d_div(leapfrog_t *res, double a,
                                                leapfrog_t *b); 
        void leapfrog_div_2(leapfrog_t *res, leapfrog_t *a); 
        void leapfrog_sqrt(leapfrog_t *res, leapfrog_t *a); 
        void leapfrog_abs(leapfrog_t *res, leapfrog_t *a);
        void leapfrog_neg(leapfrog_t *res, leapfrog_t *a); 

        /* a > b -> positive, a < b -> negative, zero also */
        int leapfrog_cmp(leapfrog_t *a, leapfrog_t *b);
        int leapfrog_cmp_double(leapfrog_t *a, double b);
#endif


#endif // LEAPFROG_MATH_H

