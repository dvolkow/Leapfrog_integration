#include <math.h>

#include "leapfrog_compile.h"
#include "leapfrog_types.h"
#include "leapfrog_generator.h"


void lp_ddot_array_zero_init(ddot_array_t *dst) 
{
        uint8_t i;

        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                dst->a[i] = 0;
        }
}


__leapfrog_hot__
double lp_point_dist(const point_t *x, 
                                const point_t *y)

{
        uint8_t         i;
        double        x_i; 
        double   dist = 0; 

        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                x_i = x->x[i] - y->x[i];
                dist += x_i * x_i;
        }
        return sqrt(dist);
}

__leapfrog_hot__
double lp_dot_product(const leapfrog_t *x,
                                const leapfrog_t *y) 
{
        uint8_t i;
        double prod = 0;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                prod += __leapfrog_t_to_double(x[i]) * 
                           __leapfrog_t_to_double(y[i]);
        }
        return prod;
}


__leapfrog_hot__
void lp_ddot_add_ddot(ddot_array_t *dst,
                        const ddot_array_t *src)
{
        uint8_t i;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                dst->a[i] += src->a[i];
        }
}

__leapfrog_hot__
void lp_ddot_mul_double(ddot_array_t *dst,
                         const double mul)
{
        uint8_t i;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                dst->a[i] *= mul;
        }
}

__leapfrog_hot__
static void lp_update_eq_x(const ddot_array_t *data,
                    equation_t *eq)
{
        uint16_t i;
        uint8_t  j;
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        GET_X(eq, i, j) = data[i].a[j];
                }
        }
}

__leapfrog_hot__
static void lp_update_eq_x_dot(const ddot_array_t *data,
                    equation_t *eq)
{
        uint16_t i;
        uint8_t  j;
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        GET_X_DOT(eq, i, j) = data[i].a[j];
                }
        }
}

__leapfrog_hot__
static void lp_update_eq_x_ddot(const ddot_array_t *data,
                    equation_t *eq)
{
        uint16_t i;
        uint8_t  j;
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        GET_X_DDOT(eq, i, j) = data[i].a[j];
                }
        }
}


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

double lp_ddot_array_norm(const ddot_array_t *array)
{
        return sqrt(lp_dot_product(array->a, array->a));
}


__leapfrog_hot__
void lp_equation_copy(equation_t *dst, const equation_t *src) 
{
        uint16_t i;
        uint8_t  j;

        lp_equation_init_shape(dst, GET_DIM(src), GET_SIZE(src));

        FORALL_BODY(i, GET_SIZE(src)) {
                FORALL_DIM(j, GET_DIM(src)) {
                        GET_X(dst, i, j)      = GET_X(src, i, j);
                        GET_X_DOT(dst, i, j)  = GET_X_DOT(src, i, j);
                        GET_X_DDOT(dst, i, j) = GET_X_DDOT(src, i, j);
                }

                GET_M(dst, i)  = GET_M(src, i);
                GET_ID(dst, i) = GET_ID(src, i);
        }
}
