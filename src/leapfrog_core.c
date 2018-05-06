#include <math.h>

#include "leapfrog_types.h"
#include "leapfrog_compile.h"
#include "leapfrog_utils.h"
#include "leapfrog_core.h"
#include "leapfrog_stats.h"
#include "leapfrog_math.h"
#include "leapfrog_precision.h"
#include "leapfrog_memory.h"
#include "leapfrog_generator.h"


#ifdef LEAPFROG_DEBUG
        #include <assert.h>
        #include <stdio.h>
#endif
        #include <stdio.h>

lp_param_t g_state;

static equation_t core_eq;
static leapfrog_t h_init;

static ddot_array_t core_ddot_i[LEAPFROG_MAX_COUNT];
static ddot_array_t core_xdot_i[LEAPFROG_MAX_COUNT];

/*
 * Return new hamiltonian before current step of integrate 
 */
__leapfrog_hot__
static void lp_optimize_step_by_hamiltonian_move(leapfrog_t *res,
                                                 equation_t *eq,
                                                 leapfrog_t *cur_step)
{
        assert(*cur_step > 0);

#ifdef LEAPFROG_DEBUG
        printf("%s: entry step %lf\n", __FUNCTION__, leapfrog_t_2_double(cur_step));
#endif
        lp_equation_copy(&core_eq, eq);
        lp_newton_update_kick_drift_kick(&core_eq, cur_step);
        lp_hamiltonian(res, &core_eq);
}


__leapfrog_hot__
static inline int lp_optimize_step_condition(leapfrog_t *h_current,
                                             leapfrog_t *h_new,
                                             leapfrog_t *precision) 
{
        int res;
        leapfrog_t cmp_value;
        LP_T_INIT(cmp_value);

        leapfrog_sub(&cmp_value, h_current, h_new);
        leapfrog_div(&cmp_value, &cmp_value, h_current);
        leapfrog_abs(&cmp_value, &cmp_value);

        res = leapfrog_cmp(&cmp_value, precision);

        LP_T_RELEASE(cmp_value);
        return res;
}

/*
 * Find optimal step 
 */
__leapfrog_hot__
void lp_optimize_step_by_hamiltonian(leapfrog_t *res, 
                                     equation_t *eq, 
                                     leapfrog_t *precision,
                                     leapfrog_t *cur_step)
{

#ifdef LEAPFROG_DEBUG
        assert(leapfrog_cmp_double(precision, 0) > 0);
        assert(leapfrog_cmp_double(cur_step, 0) > 0);
#endif

        char   was_lower = 0;
        leapfrog_t h_new;
        leapfrog_t h_current; 
        LP_T_INIT(h_new);
        LP_T_INIT(h_current);

        leapfrog_t_set_lp(res, cur_step);

        lp_optimize_step_by_hamiltonian_move(&h_new, eq, cur_step);
        lp_hamiltonian(&h_current, eq);

#ifdef LEAPFROG_DEBUG
        printf("%s: entry step %lf\n", __FUNCTION__, leapfrog_t_2_double(cur_step));
        printf("%s: hamiltonian %lf\n", __FUNCTION__, leapfrog_t_2_double(&h_init));
        printf("%s: h_new %lf\n", __FUNCTION__, leapfrog_t_2_double(&h_new));
        printf("%s: precision %0.10f\n", __FUNCTION__, leapfrog_t_2_double(precision));
#endif

        while (lp_optimize_step_condition(&h_current,
                                          &h_new, precision) > 0) {
                leapfrog_div_2(res, res);
#ifdef LEAPFROG_DEBUG
                printf("%s: res %0.15f\n", __FUNCTION__, leapfrog_t_2_double(res));
#endif
                lp_optimize_step_by_hamiltonian_move(&h_new, eq, res);
                was_lower = 1;
        }

        if (was_lower) {
                goto ret_release;
        }

        while (lp_optimize_step_condition(&h_current,
                                          &h_new, precision) < 0) {
                leapfrog_mul_2(res, res);
#ifdef LEAPFROG_DEBUG
                printf("%s: res %0.15f\n", __FUNCTION__, leapfrog_t_2_double(res));
#endif
                lp_optimize_step_by_hamiltonian_move(&h_new, eq, res);
        }

        /* 
         * Here was last cur_step * 2 
         */
        leapfrog_div_2(res, res);

ret_release:
        LP_T_RELEASE(h_current);
        LP_T_RELEASE(h_new);
}

/*
 * Center and another body
 */
static void lp_point_direction(ddot_array_t *res,
                point_t *c, point_t *a) 
{
        uint8_t i;
        for (i = 0; i < LEAPFROG_MAX_DIM; ++i) 
                leapfrog_sub(&(res->a[i]), &(a->x[i]), &(c->x[i]));
}


static void lp_newton(ddot_array_t *res,
                point_t *c, point_t *a, uint8_t dim)
{
        leapfrog_t mul, r;
        LP_T_INIT(mul);
        LP_T_INIT(r);

        /* direction set */
        lp_point_direction(res, c, a);

        lp_point_dist(&r, dim, c, a);

        leapfrog_mul(&mul, &r, &r);
        leapfrog_mul(&r, &r, &mul);

        leapfrog_mul(&mul, &(c->m), &(a->m));
        leapfrog_div(&mul, &mul, &r);
        /* F = (m * M) * r_ / (r ^ 3) 
        mul = (c->m) * 
                (a->m) / 
                  (r * r * r);
                  */
        lp_ddot_mul_leapfrog_t(res, &mul);

        LP_T_RELEASE(r);
        LP_T_RELEASE(mul);
}



__leapfrog_hot__
static void lp_full_newton(ddot_array_t *res,
                        point_t *center, equation_t *eq) 
{
        uint16_t i;
        leapfrog_t m_div;
        ddot_array_t cur_newton;

        LP_T_INIT(m_div);
        DDOT_A_INIT(&cur_newton, i);
        lp_ddot_array_zero_init(res);

        FORALL_BODY(i, GET_SIZE(eq)) {
                if (center->id != GET_ID(eq, i)) {
                        lp_newton(&cur_newton,
                                          center, &GET_BODY(eq, i), GET_DIM(eq));
                        lp_ddot_add_ddot(res, &cur_newton);
                }
        }
        leapfrog_d_div(&m_div, 1, &(center->m));
        lp_ddot_mul_leapfrog_t(res, &m_div);

        DDOT_A_RELEASE(&cur_newton, i);
        LP_T_RELEASE(m_div);
}


__leapfrog_cold__
void lp_equation_set_ddot_start(equation_t *eq) 
{
        uint16_t i;
        uint8_t  j;
        ddot_array_t ddot_start;
        DDOT_A_INIT(&ddot_start, i);

        FORALL_BODY(i, GET_SIZE(eq)) {
                lp_full_newton(&ddot_start, 
                                        &GET_BODY(eq, i), eq);
                FORALL_DIM(j, GET_DIM(eq)) {
                        leapfrog_t_set_lp(&GET_X_DDOT(eq, i, j),
                                            &(ddot_start.a[j]));
                }
        }
        DDOT_A_RELEASE(&ddot_start, i);
}


__leapfrog_hot__
void lp_newton_update_kick_drift_kick(equation_t *eq, 
                                        leapfrog_t *step)
{
        uint16_t i;
        uint8_t  j;
        leapfrog_t acc;
        LP_T_INIT(acc);

        /*
         * KICK
         * Calculate x^dot_{i + 1/2} = x^dot_i + 1/2 * x_ddot_i * step
         */
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        leapfrog_mul(&acc, &GET_X_DDOT(eq, i, j), step);
                        leapfrog_div_2(&acc, &acc);
                        leapfrog_sum(&(core_xdot_i[i].a[j]),
                                        &GET_X_DOT(eq, i, j), &acc);
                }
        }

        /* 
         * DRIFT
         * Calculate x_{i + 1} = x_i + x^dot_{i + 1/2} * step  
         */
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        leapfrog_mul(&acc, &(core_xdot_i[i].a[j]), step);
                        leapfrog_sum(&GET_X(eq, i, j), &GET_X(eq, i, j), &acc);
                }
        }


        FORALL_BODY(i, GET_SIZE(eq)) {
                 lp_full_newton(&core_ddot_i[i], &GET_BODY(eq, i), eq);
        }

        /*
         * KICK
         * Calculate x^dot_{i + 1} = x^dot_{i + 1/2} + 1/2 * x_ddot_i * step
         */
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        leapfrog_mul(&acc, &(core_ddot_i[i].a[j]), step);
                        leapfrog_div_2(&acc, &acc);
                        leapfrog_sum(&GET_X_DOT(eq, i, j), 
                                        &(core_xdot_i[i].a[j]), &acc);
                }
        }

        lp_update_eq_generic(core_ddot_i, eq, LP_XDDOT);
        LP_T_RELEASE(acc);
}

__leapfrog_hot__
void lp_newton_update_eq(equation_t *eq, leapfrog_t *step)
{
        uint16_t i;
        uint8_t  j;
        leapfrog_t acc;
        ddot_array_t ddot_i[LEAPFROG_MAX_COUNT];
        ddot_array_t x_i[LEAPFROG_MAX_COUNT];
        ddot_array_t xdot_i[LEAPFROG_MAX_COUNT];

        FORALL_BODY(i, GET_SIZE(eq)) {
                lp_full_newton(&core_ddot_i[i], &GET_BODY(eq, i), eq);
        }

        /* 
         * Calculate x_{i + 1} = x_i + x^dot_i * step + 1/2 * ddot_i * step ^ 2 
         */
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        leapfrog_mul(&(x_i[i].a[j]), step, step);
                        leapfrog_mul(&(x_i[i].a[j]), &GET_X_DDOT(eq, i, j), &(x_i[i].a[j]));
                        leapfrog_div_2(&(x_i[i].a[j]), &(x_i[i].a[j]));
                        leapfrog_sum(&(x_i[i].a[j]), &GET_X(eq, i, j), &(x_i[i].a[j]));
                        leapfrog_mul(&acc, &GET_X_DOT(eq, i, j), step);
                        leapfrog_sum(&(x_i[i].a[j]), &(x_i[i].a[j]), &acc);
                }
        }

        /*
         * Calculate x^dot_{i + 1} = x^dot_i + 1/2 * (x_ddot_i + x_ddot_{i + 1}) * step
         */
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        leapfrog_sum(&(x_i[i].a[j]), &GET_X_DDOT(eq, i, j), &(ddot_i[i].a[j]));
                        leapfrog_mul(&(x_i[i].a[j]), step, &(x_i[i].a[j]));
                        leapfrog_div_2(&(x_i[i].a[j]), &(x_i[i].a[j]));
                        leapfrog_sum(&(xdot_i[i].a[j]), &GET_X_DOT(eq, i, j), &(x_i[i].a[j]));
                }
        }

        lp_update_eq_generic(x_i, eq, LP_X);
        lp_update_eq_generic(xdot_i, eq, LP_XDOT);
        lp_update_eq_generic(ddot_i, eq, LP_XDDOT);
}


__leapfrog_hot__
void lp_core_up(void) 
{
#ifdef LEAPFROG_DEBUG
        assert(leapfrog_cmp_double(&g_state.step, 0) > 0);
#endif
        lp_optimize_step_by_hamiltonian(&g_state.step, &g_state.eq, &g_state.precision, &g_state.step);
        lp_newton_update_kick_drift_kick(&g_state.eq, &g_state.step);
}

__leapfrog_cold__
void lp_core_hamiltonian_init(leapfrog_t *h) 
{
        leapfrog_t_set_lp(&h_init, h);
}

__leapfrog_cold__ 
void lp_core_structures_init(void) 
{
        uint16_t i, j;
        FORALL_BODY(i, LEAPFROG_MAX_COUNT) {
                DDOT_A_INIT(&core_ddot_i[i], j);
                DDOT_A_INIT(&core_xdot_i[i], j);
        }
        LP_T_INIT(h_init);
        lp_equation_init(&core_eq);
        lp_param_t_init(&g_state);
}

__leapfrog_cold__ 
void lp_core_structures_release(void)
{
        uint16_t i, j;
        lp_param_t_release(&g_state);
        LP_T_RELEASE(h_init);
        lp_equation_release(&core_eq);
        FORALL_BODY(i, LEAPFROG_MAX_COUNT) {
                DDOT_A_RELEASE(&core_xdot_i[i], j);
                DDOT_A_RELEASE(&core_ddot_i[i], j);
        }
}
