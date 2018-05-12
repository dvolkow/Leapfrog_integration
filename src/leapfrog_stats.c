/*
 * Here calculate basic statistic
 * of global structures. Most of
 * them is Hamiltonial, that use
 * for balance integrate step.
 * Also may be include another 
 * features for monitoring of sta-
 * tements.
 */
#include "leapfrog_types.h"
#include "leapfrog_compile.h"
#include "leapfrog_utils.h"
#include "leapfrog_math.h"
#include "leapfrog_memory.h"

static leapfrog_t stats_dist;
static leapfrog_t stats_K;
static leapfrog_t stats_P;
static leapfrog_t stats_E;


__leapfrog_hot__
static void lp_potential(leapfrog_t *res, 
                        point_t *c, point_t *a, uint8_t dim)
{
        leapfrog_mul(res, &(c->m), &(a->m));
        lp_point_dist(&stats_dist, dim, c, a);
        leapfrog_neg(res, res);
        leapfrog_div(res, res, &stats_dist);
}


__leapfrog_hot__
static void lp_full_potential(leapfrog_t *res,
                                equation_t *eq)
{
        uint16_t i, j;
        leapfrog_t_set_d(res, 0);

        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_BODY(j, GET_SIZE(eq)) {
                        if (i != j) {
                                lp_potential(&stats_P, &GET_BODY(eq, i), 
                                                &GET_BODY(eq, j), GET_DIM(eq));
                                leapfrog_sum(res, res, &stats_P);
                        }
                }
        }

        leapfrog_div_2(res, res);
}


__leapfrog_hot__
static void lp_full_kinetic_energy(leapfrog_t *res, 
                                        equation_t *eq)
{
        uint16_t i;

        leapfrog_t_set_d(res, 0);
        leapfrog_t_set_d(&stats_K, 0);

        FORALL_BODY(i, GET_SIZE(eq)) {
                lp_dot_product(&stats_K, GET_DIM(eq), 
                                GET_BODY(eq, i).x_dot, GET_BODY(eq, i).x_dot);
                leapfrog_mul(&stats_K, &GET_M(eq, i), &stats_K);
                leapfrog_div_2(&stats_K, &stats_K);
                leapfrog_sum(res, res, &stats_K);
        }
}


/*
 * Basic control by this
 */
__leapfrog_hot__
void lp_hamiltonian(leapfrog_t *res, equation_t *eq)
{
        leapfrog_t_set_d(&stats_E, 0);

        lp_full_kinetic_energy(&stats_E, eq);
        lp_full_potential(res, eq);

        leapfrog_sum(res, res, &stats_E);
}

__leapfrog_cold__ 
void lp_stats_init(void) 
{
        LP_T_INIT(stats_K);
        LP_T_INIT(stats_P);
        LP_T_INIT(stats_E);
        LP_T_INIT(stats_dist);
}

__leapfrog_cold__ 
void lp_stats_release(void) 
{
        LP_T_RELEASE(stats_dist);
        LP_T_RELEASE(stats_K);
        LP_T_RELEASE(stats_P);
        LP_T_RELEASE(stats_E);
}
