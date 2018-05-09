/*
 * Utility operations that 
 * often used for temporary
 * calculations. Something
 * from it may be reused or
 * refactor in future.
 * Also here velocycles is.
 */
#include <math.h>

#include "leapfrog_compile.h"
#include "leapfrog_types.h"
#include "leapfrog_utils.h"
#include "leapfrog_generator.h"
#include "leapfrog_precision.h"
#include "leapfrog_math.h"
#include "leapfrog_memory.h"

extern lp_param_t g_state;


__leapfrog_cold__
void lp_ddot_array_zero_init(ddot_array_t *dst) 
{
        uint8_t i;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) 
                leapfrog_t_set_i(&GET_DDOT_A(dst, i), 0);
}


static inline void lp_point_init_by_point(point_t *dst, point_t *src) 
{
        point_id_t i;
        FORALL_DIM(i, LEAPFROG_MAX_DIM) {
                leapfrog_t_set_lp(&dst->x[i], &src->x[i]);
                leapfrog_t_set_lp(&dst->x_dot[i], &src->x_dot[i]);
                leapfrog_t_set_lp(&dst->x_ddot_prev[i], 
                                  &src->x_ddot_prev[i]);
        }
        leapfrog_t_set_lp(&dst->m, &src->m);
        /* 
         * ID is not attribute! 
         * Mean nothing without equaiton 
         */
}


/* Generating bodies made me write is */
void lp_push_body_to_eq(point_t *body, equation_t *eq)
{
        assert(GET_SIZE(eq) < LEAPFROG_MAX_COUNT);
        /* Equation is same stack */
        ++GET_SIZE(eq);
        lp_point_init_by_point(&GET_BODY(eq, GET_SIZE(eq) - 1),
                                                          body);
}


/*
 * @shift_array: contains shift vector
 * @dim        : is length of shift_array 
 */
void lp_body_shift_x(point_t *body, leapfrog_t *shift_array, 
                                                const uint8_t dim)
{
        uint8_t i;
        FORALL_DIM(i, dim) {
                leapfrog_sum(&body->x[i], &body->x[i], 
                /* need &shift[i] */      shift_array + i);
        }
}


/* Typical stack interface */
void lp_pop_body_from_eq(equation_t *eq) 
{
        assert(GET_SIZE(eq) > 1);
        --GET_SIZE(eq);
        /* Because access to eq elements
         * call GET_SIZE(eq) in cycles,
         * after this last body will not
         * reached
         */
}


/*
 * Calculate distantion between two points
 * Result put to @res.
 */
__leapfrog_hot__
void lp_point_dist(leapfrog_t *res, uint8_t dim,
                    point_t *x, point_t *y)

{
        uint8_t         i;
        leapfrog_t    x_i; 
        leapfrog_t   dist; 

        LP_T_INIT(x_i);
        LP_T_INIT(dist);


        leapfrog_t_set_i(&dist, 0);

        for (i = 0; i < dim; ++i) {
                leapfrog_sub(&x_i, &(x->x[i]), &(y->x[i]));
                leapfrog_mul(&x_i, &x_i, &x_i);
                leapfrog_sum(&dist, &dist, &x_i);
        }

        leapfrog_sqrt(res, &dist);

        LP_T_RELEASE(dist);
        LP_T_RELEASE(x_i);
}



/* 
 * Calculate dot product between two vector
 * Result put to @res.
 */
__leapfrog_hot__
void lp_dot_product(leapfrog_t *res, uint8_t dim,
                        leapfrog_t *x, leapfrog_t *y) 
{
        uint8_t i;
        leapfrog_t prod;

        LP_T_INIT(prod);
        leapfrog_t_set_i(res, 0);

        for (i = 0; i < dim; ++i) {
                leapfrog_mul(&prod, (x + i), (y + i));
                leapfrog_sum(res, res, &prod);
        }
        LP_T_RELEASE(prod);
}


/*
 * Sum of vectors @dst and @src puts into 
 * @dst
 */
__leapfrog_hot__
void lp_ddot_add_ddot(ddot_array_t *dst,
                        const ddot_array_t *src)
{
        uint8_t i;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                leapfrog_sum(&GET_DDOT_A(dst, i),
                                &GET_DDOT_A(dst, i), 
                                   &GET_DDOT_A(src, i));
        }
}


/*
 * Array multiply by @mul 
 * put to dst (on place)
 */
__leapfrog_hot__
inline void lp_ddot_mul_leapfrog_t(ddot_array_t *dst,
                                leapfrog_t *mul)
{
        uint8_t i;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                leapfrog_mul(&GET_DDOT_A(dst, i), 
                                &GET_DDOT_A(dst, i), mul);
        }
}


/*
 * Set equation's x fields by array of 
 * arrays with values.
 */
__leapfrog_hot__
static void lp_update_eq_x(const ddot_array_t *data,
                    equation_t *eq)
{
        uint16_t i;
        uint8_t  j;
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        leapfrog_t_set_lp(&GET_X(eq, i, j),
                                        &(data[i].a[j]));
                }
        }
}


/*
 * Set equation's x_dot fields by array of 
 * arrays with values.
 */
__leapfrog_hot__
static void lp_update_eq_x_dot(const ddot_array_t *data,
                    equation_t *eq)
{
        uint16_t i;
        uint8_t  j;
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        leapfrog_t_set_lp(&GET_X_DOT(eq, i, j),
                                        &(data[i].a[j]));
                }
        }
}


/*
 * Set equation's x-double-dot fields by array of 
 * arrays with values.
 */
__leapfrog_hot__
static void lp_update_eq_x_ddot(const ddot_array_t *data,
                    equation_t *eq)
{
        uint16_t i;
        uint8_t  j;
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        leapfrog_t_set_lp(&GET_X_DDOT(eq, i, j),
                                        &(data[i].a[j]));
                }
        }
}


/*
 * Fetch needed update function
 * Generic.
 */
__leapfrog_hot__
void lp_update_eq_generic(const ddot_array_t *data, 
                          equation_t *eq, 
                          const int type) 
{
        switch(type) {
                case(LP_X):
                        lp_update_eq_x(data, eq);
                        break;
                case(LP_XDOT):
                        lp_update_eq_x_dot(data, eq);
                        break;
                case(LP_XDDOT):
                        lp_update_eq_x_ddot(data, eq);
                        break;
        }
}


/*
 * Calculate 'length' of @array, 
 * and put into @res. 
 *      @dim is count of dimentions
 */
__leapfrog_hot__
void lp_ddot_array_norm(leapfrog_t *res, uint8_t dim,
                        ddot_array_t *array)
{
        lp_dot_product(res, dim, array->a, array->a);
        leapfrog_sqrt(res, res);
}


/*
 * Copy constructor for equation type
 */
__leapfrog_hot__
void lp_equation_copy(equation_t *dst, const equation_t *src) 
{
        uint16_t i;
        uint8_t  j;

        lp_equation_init_shape(dst, GET_DIM(src), GET_SIZE(src));

        FORALL_BODY(i, GET_SIZE(src)) {
                FORALL_DIM(j, GET_DIM(src)) {
                        leapfrog_t_set_lp(&GET_X(dst, i, j), &GET_X(src, i, j));
                        leapfrog_t_set_lp(&GET_X_DOT(dst, i, j), &GET_X_DOT(src, i, j));
                        leapfrog_t_set_lp(&GET_X_DDOT(dst, i, j), &GET_X_DDOT(src, i, j));
                }

                leapfrog_t_set_lp(&GET_M(dst, i), &GET_M(src, i));
                GET_ID(dst, i) = GET_ID(src, i);
        }
}

__leapfrog_cold__
void lp_init_state_precision(void) 
{
        uint8_t i;
        leapfrog_t bits_count;
        LP_T_INIT(bits_count);

        leapfrog_t_set_d(&bits_count, 1);
        if (g_state.precision_type == LP_DEC_ROUNDING) {
                for (i = 0; i < g_state.precision_rounding; ++i) {
                        leapfrog_mul_d(&bits_count, 
                                        &bits_count, 10);
                }
        } else if (g_state.precision_type == LP_BITS_ROUNDING) {
                for (i = 0; i < g_state.precision_bits; ++i) {
                        leapfrog_mul_2(&bits_count, &bits_count);
                } 
        } else {
                assert("Bad precision_type\n");
        }


        leapfrog_d_div(&bits_count, 1, &bits_count);
        leapfrog_t_set_lp(&g_state.precision, &bits_count);
        
#ifdef LEAPFROG_DEBUG
        printf("Set precision: %0.15f\n", leapfrog_t_2_double(&g_state.precision));
#endif

        LP_T_RELEASE(bits_count);
}

/*  *******************   Multiprecition part  *******************   */
#ifdef LP_MPFR_CASE
#include <mpfr.h>

__leapfrog_hot__
void leapfrog_t_set_d(leapfrog_t *res, const double d) 
{
        mpfr_set_d(*res, d, LP_RND);
}

__leapfrog_hot__
void leapfrog_t_set_i(leapfrog_t *res, const unsigned i) 
{
        mpfr_set_ui(*res, i, LP_RND);
}

__leapfrog_hot__
void leapfrog_t_set_lp(leapfrog_t *res, const leapfrog_t *l)
{
        mpfr_set(*res, *l, LP_RND);
}

__leapfrog_hot__ __leapfrog_pure__
double leapfrog_t_2_double(leapfrog_t *l) 
{
        return mpfr_get_d(*l, LP_RND);
}

#else 
__leapfrog_hot__
void leapfrog_t_set_d(leapfrog_t *res, const double d) 
{
        *res = d;
}

__leapfrog_hot__
void leapfrog_t_set_i(leapfrog_t *res, const unsigned i) 
{
        *res = i;
}

__leapfrog_hot__
void leapfrog_t_set_lp(leapfrog_t *res, const leapfrog_t *l)
{
        *res = *l;
}


__leapfrog_hot__ __leapfrog_pure__
double leapfrog_t_2_double(leapfrog_t *l) 
{
        return (double)(*l);
}

#endif // LP_MPFR_CASE

