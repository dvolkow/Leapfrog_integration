#include "leapfrog_types.h"
#include "leapfrog_compile.h"
#include "leapfrog_utils.h"
#include "leapfrog_math.h"
#include "leapfrog_memory.h"



__leapfrog_hot__
static void lp_potential(leapfrog_t *res, 
                        point_t *c, point_t *a, uint8_t dim)
{
        leapfrog_t dist;
        LP_T_INIT(dist);

        leapfrog_mul(res, &(c->m), &(a->m));
        lp_point_dist(&dist, dim, c, a);
        leapfrog_neg(res, res);
        leapfrog_div(res, res, &dist);

        LP_T_RELEASE(dist);
}

__leapfrog_hot__
static void lp_full_potential(leapfrog_t *res,
                                equation_t *eq)
{
        uint16_t i, j;
        leapfrog_t P;
        LP_T_INIT(P);
        leapfrog_t_set_d(res, 0);

        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_BODY(j, GET_SIZE(eq)) {
                        if (i != j) {
                                lp_potential(&P, &GET_BODY(eq, i), 
                                                &GET_BODY(eq, j), GET_DIM(eq));
                                leapfrog_sum(res, res, &P);
                        }
                }
        }

        leapfrog_div_2(res, res);
        LP_T_RELEASE(P);
}


__leapfrog_hot__
static void lp_full_kinetic_energy(leapfrog_t *res, 
                                        equation_t *eq)
{
        uint16_t i;
        leapfrog_t E;
        LP_T_INIT(E);

        leapfrog_t_set_d(res, 0);
        leapfrog_t_set_d(&E, 0);

        FORALL_BODY(i, GET_SIZE(eq)) {
                lp_dot_product(&E, GET_DIM(eq), 
                                GET_BODY(eq, i).x_dot, GET_BODY(eq, i).x_dot);
                leapfrog_mul(&E, &GET_M(eq, i), &E);
                leapfrog_div_2(&E, &E);
                leapfrog_sum(res, res, &E);
        }

        LP_T_RELEASE(E);
}

__leapfrog_hot__
void lp_hamiltonian(leapfrog_t *res, equation_t *eq)
{
        leapfrog_t K;
        LP_T_INIT(K);

        leapfrog_t_set_d(&K, 0);

        lp_full_kinetic_energy(&K, eq);
        lp_full_potential(res, eq);

        leapfrog_sum(res, res, &K);
        LP_T_RELEASE(K);
}
