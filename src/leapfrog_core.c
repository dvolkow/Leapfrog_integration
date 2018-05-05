#include "leapfrog_types.h"
#include "leapfrog_compile.h"
#include "leapfrog_utils.h"
#include "leapfrog_core.h"
#include "leapfrog_stats.h"

//#include "leapfrog.h"
#include <math.h>

#ifdef LEAPFROG_DEBUG
        #include <assert.h>
        #include <stdio.h>
#endif

static equation_t core_eq;
static double h_init = 0;

/*
 * Return new hamiltonian before current step of integrate 
 */
__leapfrog_hot__
static double lp_optimize_step_by_hamiltonian_move(const equation_t *eq,
                                                const double cur_step)
{
        assert(cur_step > 0);

#ifdef LEAPFROG_DEBUG
        printf("%s: entry step %lf\n", __FUNCTION__, cur_step);
#endif
        lp_equation_copy(&core_eq, eq);
        lp_newton_update_kick_drift_kick(&core_eq, cur_step);
        return lp_hamiltonian(&core_eq);
}

/*
 * Find optimal step 
 */
__leapfrog_hot__
double lp_optimize_step_by_hamiltonian(const equation_t *eq, 
                                              const double precision,
                                              double cur_step)
{
        assert(precision > 0);

        char   was_lower = 0;
        double h_new = lp_optimize_step_by_hamiltonian_move(eq, cur_step);
        double h_current = lp_hamiltonian(eq);

#ifdef LEAPFROG_DEBUG
        printf("%s: entry step %lf\n", __FUNCTION__, cur_step);
        printf("%s: hamiltonian %lf\n", __FUNCTION__, h_init);
        printf("%s: h_new %lf\n", __FUNCTION__, h_new);
        printf("%s: precision %lf\n", __FUNCTION__, precision);
#endif

        while (1 - fabs(h_current/h_new) > precision) {
                cur_step /= 2.;
#ifdef LEAPFROG_DEBUG
                printf("%s: new step test %lf\n", __FUNCTION__, cur_step);
                printf("%s: condition: %lf\n", __FUNCTION__, 1 - fabs(h_current/h_new));
#endif
                h_new = lp_optimize_step_by_hamiltonian_move(eq, cur_step);
                was_lower = 1;
        }

        if (was_lower) 
                return cur_step;

        while (fabs(1 - fabs(h_current/h_new)) < precision) {
                cur_step *= 2.;
                h_new = lp_optimize_step_by_hamiltonian_move(eq, cur_step);
        }

        /* 
         * Here was last cur_step * 2 
         */
        return  cur_step / 2;
}

/*
 * Center and another body
 */
static ddot_array_t lp_point_direction(const point_t *c,
                                const point_t *a) 
{
        uint8_t i;
        ddot_array_t direct;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) {
                direct.a[i] = (a->x[i]) - 
                        (c->x[i]);
        }

        return direct;
}


static ddot_array_t lp_newton(const point_t *c,
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


static leapfrog_t lp_new_x(const leapfrog_t x, 
                    const leapfrog_t x_dot, 
                    const leapfrog_t step)
{
        /* TODO: types correct! */
        return __leapfrog_t_to_double(x) + 
                __leapfrog_t_to_double(step) * 
                  __leapfrog_t_to_double(x_dot);
}


__leapfrog_hot__
static ddot_array_t lp_full_newton(point_t *center,
                                equation_t *eq) 
{
        uint16_t i;
        ddot_array_t res, cur_newton;
        lp_ddot_array_zero_init(&res);

        FORALL_BODY(i, GET_SIZE(eq)) {
                if (center->id != GET_ID(eq, i)) {
                        cur_newton = lp_newton(center, &GET_BODY(eq, i));
                        lp_ddot_add_ddot(&res, &cur_newton);
                }
        }
        lp_ddot_mul_double(&res, 1 / center->m);
        return res;
}


__leapfrog_cold__
void lp_equation_init(equation_t *eq) 
{
        uint16_t i;
        uint8_t  j;
        ddot_array_t ddot_start;

        FORALL_BODY(i, GET_SIZE(eq)) {
                ddot_start = lp_full_newton(&GET_BODY(eq, i), eq);
                FORALL_DIM(j, GET_DIM(eq)) {
                        GET_X_DDOT(eq, i, j) = ddot_start.a[j];
                }
        }
}


__leapfrog_hot__
void lp_newton_update_kick_drift_kick(equation_t *eq, 
                                        const double step)
{
        uint16_t i;
        uint8_t  j;
        ddot_array_t ddot_i[LEAPFROG_MAX_COUNT];
        ddot_array_t xdot_i[LEAPFROG_MAX_COUNT];

        /*
         * KICK
         * Calculate x^dot_{i + 1/2} = x^dot_i + 1/2 * x_ddot_i * step
         */
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        xdot_i[i].a[j] = GET_X_DOT(eq, i, j) + 
                                         0.5 * GET_X_DDOT(eq, i, j) * step;
                }
        }

        /* 
         * DRIFT
         * Calculate x_{i + 1} = x_i + x^dot_{i + 1/2} * step  
         */
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        GET_X(eq, i, j) = GET_X(eq, i, j) + xdot_i[i].a[j] * step;
                }
        }


        FORALL_BODY(i, GET_SIZE(eq)) {
                ddot_i[i] = lp_full_newton(&GET_BODY(eq, i), eq);
        }

        /*
         * KICK
         * Calculate x^dot_{i + 1} = x^dot_{i + 1/2} + 1/2 * x_ddot_i * step
         */
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        GET_X_DOT(eq, i, j) = xdot_i[i].a[j] + 
                                         0.5 * ddot_i[i].a[j] * step;
                }
        }

        lp_update_eq_generic(ddot_i, eq, LP_XDDOT);
}

__leapfrog_hot__
void lp_newton_update_eq(equation_t *eq, const double step)
{
        uint16_t i;
        uint8_t  j;
        ddot_array_t ddot_i[LEAPFROG_MAX_COUNT];
        ddot_array_t x_i[LEAPFROG_MAX_COUNT];
        ddot_array_t xdot_i[LEAPFROG_MAX_COUNT];

        FORALL_BODY(i, GET_SIZE(eq)) {
                ddot_i[i] = lp_full_newton(&GET_BODY(eq, i), eq);
        }

        /* 
         * Calculate x_{i + 1} = x_i + x^dot_i * step + 1/2 * ddot_i * step ^ 2 
         */
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        x_i[i].a[j] = GET_X(eq, i, j) + GET_X_DOT(eq, i, j) * step +
                                              0.5 * GET_X_DDOT(eq, i, j) * step * step;
                }
        }

        /*
         * Calculate x^dot_{i + 1} = x^dot_i + 1/2 * (x_ddot_i + x_ddot_{i + 1}) * step
         */
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        xdot_i[i].a[j] = GET_X_DOT(eq, i, j) + 
                                         0.5 * (GET_X_DDOT(eq, i, j) + ddot_i[i].a[j]) * step;
                }
        }

        lp_update_eq_generic(x_i, eq, LP_X);
        lp_update_eq_generic(xdot_i, eq, LP_XDOT);
        lp_update_eq_generic(ddot_i, eq, LP_XDDOT);
}


__leapfrog_cold__
void lp_core_hamiltonian_init(const double h) 
{
        h_init = h;
}
