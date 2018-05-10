/*
 * Interface to utility operations
 * and procedures for LP.
 * Most of this need rewrite and re-
 * factor
 */
#ifndef LEAPFROG_UTILS 
#define LEAPFROG_UTILS  1

#include "leapfrog_types.h"

/* Initializations: */
void leapfrog_t_set_d(leapfrog_t *, const double d);
void leapfrog_t_set_i(leapfrog_t *, const unsigned i);
void leapfrog_t_set_lp(leapfrog_t *, const leapfrog_t *);

void lp_init_state_precision(void);
void lp_ddot_array_zero_init(ddot_array_t *);


/* Body target operation:
 * -- push/pop from equation
 * -- shift
 */
void lp_push_body_to_eq(point_t *body, equation_t *eq);
void lp_pop_body_from_eq(equation_t *eq); 

/* Move body by space coordinates */
void lp_body_shift_x(point_t *body, leapfrog_t *shift_array, 
                                        const uint8_t dim);


/*
 * Calculate distantion between points
 */
void lp_point_dist(leapfrog_t *res, uint8_t dim,
                    point_t *x, point_t *y);

/*
 * Dot product for two vector of leapfrog_t
 */
void lp_dot_product(leapfrog_t *res, uint8_t dim,
                        leapfrog_t *, leapfrog_t *);

/*
 * Addiction double-dot arrays
 */
void lp_ddot_add_ddot(ddot_array_t *, const ddot_array_t *);

/*
 * Multiply array by constant
 */
void lp_ddot_mul_leapfrog_t(ddot_array_t *, leapfrog_t *);

/*
 * "Length"
 */
void lp_ddot_array_norm(leapfrog_t *res, uint8_t dim,
                        ddot_array_t *);

/*
 * Set eq fields
 */
void lp_update_eq_generic(const ddot_array_t *, equation_t *, const int);

/*
 * Copy equation_t
 */
void lp_equation_copy(equation_t *dst, const equation_t *src);

/* Conversion */
double leapfrog_t_2_double(leapfrog_t *);

#endif // LEAPFROG_UTILS
