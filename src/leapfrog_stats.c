#include "leapfrog_types.h"
#include "leapfrog_compile.h"
#include "leapfrog_utils.h"

__leapfrog_hot__
static double lp_potential(const point_t *c, const point_t *a)
{
        return -(c->m * a->m) / lp_point_dist(c, a);
}

__leapfrog_hot__
static double lp_full_potential(const equation_t *eq)
{
        uint16_t i, j;
        double   P = 0;

        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_BODY(j, GET_SIZE(eq)) {
                        if (i != j) {
                                P += lp_potential(&GET_BODY(eq, i), &GET_BODY(eq, j));
                        }
                }
        }

        return P / 2;
}


__leapfrog_hot__
static double lp_full_kinetic_energy(const equation_t *eq)
{
        uint16_t i;
        double   E = 0;

        FORALL_BODY(i, GET_SIZE(eq)) {
                E += GET_M(eq, i) * lp_dot_product(GET_BODY(eq, i).x_dot, 
                                                   GET_BODY(eq, i).x_dot) / 2;
        }
        return E;
}

__leapfrog_hot__
double lp_hamiltonian(const equation_t *eq)
{
        return lp_full_potential(eq) + lp_full_kinetic_energy(eq);
}
