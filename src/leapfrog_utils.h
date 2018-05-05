#ifndef LEAPFROG_UTILS 
#define LEAPFROG_UTILS  1

#include "leapfrog_types.h"

/*
 * Initial double-dot array by zeros
 */
void lp_ddot_array_zero_init(ddot_array_t *);

/*
 * Calculate distantion between points
 */
double lp_point_dist(const point_t *, const point_t *);

/*
 * Dot product for two vector of leapfrog_t
 */
double lp_dot_product(const leapfrog_t *, const leapfrog_t *);

/*
 * Addiction double-dot arrays
 */
void lp_ddot_add_ddot(ddot_array_t *, const ddot_array_t *);

/*
 * Multiply array by constant
 */
void lp_ddot_mul_double(ddot_array_t *, const double);

/*
 * "Length"
 */
double lp_ddot_array_norm(const ddot_array_t *);

/*
 * Set eq fields
 */
void lp_update_eq_generic(const ddot_array_t *, equation_t *, const int);

/*
 * Copy equation_t
 */
void lp_equation_copy(equation_t *dst, const equation_t *src);

#endif // LEAPFROG_UTILS
