/*
 * Math ops that abstract us from 
 * real types that used here. You
 * can use your type as 'leapfrog_t'
 * if to define operations below:
 */
#include "leapfrog_math.h"

#ifdef LP_MPFR_CASE

__leapfrog_hot__ __leapfrog_const__
double leapfrog_to_double(leapfrog_t *lp)
{
        return mpfr_get_d(*lp, LP_RND);
}


__leapfrog_hot__
inline void leapfrog_sum(leapfrog_t *res, leapfrog_t *a, const leapfrog_t *b) 
{
        mpfr_add(*res, *a, *b, LP_RND);
}


__leapfrog_hot__
inline void leapfrog_sub(leapfrog_t *res, leapfrog_t *a, leapfrog_t *b) 
{
        mpfr_sub(*res, *a, *b, LP_RND);
}

__leapfrog_hot__
inline void leapfrog_mul(leapfrog_t *res, leapfrog_t *a, leapfrog_t *b) 
{
        mpfr_mul(*res, *a, *b, LP_RND);
}

__leapfrog_cold__
inline void leapfrog_mul_d(leapfrog_t *res, leapfrog_t *a, double b) 
{
        mpfr_mul_d(*res, *a, b, LP_RND);
}

__leapfrog_hot__
inline void leapfrog_mul_2(leapfrog_t *res, leapfrog_t *a) 
{
        mpfr_mul_2ui(*res, *a, 1, LP_RND);
}

__leapfrog_hot__
inline void leapfrog_div(leapfrog_t *res, leapfrog_t *a, leapfrog_t *b) 
{
        mpfr_div(*res, *a, *b, LP_RND);
}

__leapfrog_hot__
inline void leapfrog_d_div(leapfrog_t *res, double a,
                                                leapfrog_t *b)
{
        mpfr_d_div(*res, a, *b, LP_RND);
}

__leapfrog_hot__
inline void leapfrog_div_d(leapfrog_t *res, 
                                leapfrog_t *a, double b)
{
        mpfr_div_d(*res, *a, b, LP_RND);
}

__leapfrog_hot__
inline void leapfrog_div_2(leapfrog_t *res, leapfrog_t *a) 
{
        mpfr_div_2ui(*res, *a, 1, LP_RND);
}

__leapfrog_hot__
inline void leapfrog_sqrt(leapfrog_t *res, leapfrog_t *a) 
{
        mpfr_sqrt(*res, *a, LP_RND);
}

__leapfrog_hot__
inline void leapfrog_abs(leapfrog_t *res, leapfrog_t *a) 
{
        mpfr_abs(*res, *a, LP_RND);
}

__leapfrog_hot__
inline void leapfrog_neg(leapfrog_t *res, leapfrog_t *a) 
{
        mpfr_neg(*res, *a, LP_RND);
}

/* Comparison */
__leapfrog_hot__ __leapfrog_const__
inline int leapfrog_cmp(leapfrog_t *a, leapfrog_t *b)
{
        return mpfr_cmp(*a, *b);
}

__leapfrog_hot__ __leapfrog_const__
inline int leapfrog_cmp_double(leapfrog_t *a, double b)
{
        return mpfr_cmp_d(*a, b);
}

#endif
