#include <math.h>
#include "leapfrog.h"
#include "leapfrog_types.h"


void lp_ddot_array_zero_init(ddot_array_t *dst) 
{
        uint8_t i;

        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                dst->a[i] = 0;
        }
}

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


double lp_dot_product(leapfrog_t *x, leapfrog_t *y) 
{
        uint8_t i;
        double prod = 0;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                prod += __leapfrog_t_to_double(x[i]) * 
                           __leapfrog_t_to_double(y[i]);
        }
        return prod;
}


void lp_ddot_add_ddot(ddot_array_t *dst,
                        const ddot_array_t *src)
{
        uint8_t i;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                dst->a[i] += src->a[i];
        }
}

void lp_ddot_mul_double(ddot_array_t *dst,
                         const double mul)
{
        uint8_t i;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                dst->a[i] *= mul;
        }
}

/*
 * Center and another body
 */
ddot_array_t lp_point_direction(const point_t *c,
                                const point_t *a) 
{
        uint8_t i;
        ddot_array_t direct;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                direct.a[i] = __leapfrog_t_to_double(c->x[i]) - 
                        __leapfrog_t_to_double(a->x[i]);
        }

        return direct;
}


ddot_array_t lp_newton(const point_t *c,
                                const point_t *a)
{
        double  mul, r;
        ddot_array_t res;

        /* direction set */
        res = lp_point_direction(c, a);

        r = lp_point_dist(c, a);

        /* F = (m * M) * r_ / (r ^ 3) */
        mul = __leapfrog_t_to_double(c->m) * 
                __leapfrog_t_to_double(a->m) / 
                  (r * r * r);

        lp_ddot_mul_double(&res, mul);
        return res;
}

leapfrog_t lp_new_x(const leapfrog_t x, 
                    const leapfrog_t x_dot, 
                    const leapfrog_t step)
{
        /* TODO: types correct! */
        return __leapfrog_t_to_double(x) + 
                __leapfrog_t_to_double(step) * 
                  __leapfrog_t_to_double(x_dot);
}

ddot_array_t lp_full_newton(point_t *center,
                                equation_t *eq) 
{
        uint16_t i;
        ddot_array_t res, cur_newton;
        lp_ddot_array_zero_init(&res);

        for (i = 0; i < LEAPFROG_MAX_COUNT; ++i) {
                cur_newton = lp_newton(center, &(eq->body[i]));
                lp_ddot_add_ddot(&res, &cur_newton);
        }
        return res;
}


void lp_equation_init(equation_t *eq) 
{
        uint16_t i;
        uint8_t  j;
        ddot_array_t ddot_start;

        for (i = 0; i < LEAPFROG_MAX_COUNT; ++i) {
                ddot_start = lp_full_newton(&eq->body[i], eq);
                for (j = 0; j < LEAPFROG_MAX_DIM; ++j) {
                        (eq->body[i]).x_ddot_prev[j] = ddot_start.a[j];
                }
        }
}


ddot_array_t lp_point_x_i_calc(const point_t *center,
                                        const equation_t *eq) 
{
        uint8_t i;
        ddot_array_t x_i;
        lp_ddot_array_zero_init(&x_i);

        return x_i;
}

void lp_newton_update_eq(equation_t *eq)
{
        uint16_t i;
        // uint8_t  j;
        ddot_array_t ddot_i[LEAPFROG_MAX_COUNT];
        // ddot_array_t x_i[LEAPFROG_MAX_COUNT];
        // ddot_array_t xdot_i[LEAPFROG_MAX_COUNT];

        for (i = 0; i < LEAPFROG_MAX_COUNT; ++i) {
                ddot_i[i] = lp_full_newton(&eq->body[i], eq);
        }

}


