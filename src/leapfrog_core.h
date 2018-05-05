#ifndef LEAPFROG_CORE_H
#define LEAPFROG_CORE_H  1

#include "leapfrog_types.h"

void lp_equation_init(equation_t *eq);
void lp_newton_update_eq(equation_t *eq, const double step);

double lp_optimize_step_by_hamiltonian(const equation_t *eq, 
                                       const double precision,
                                             double cur_step);

void lp_core_hamiltonian_init(const double); 

void lp_newton_update_kick_drift_kick(equation_t *eq, 
                                        const double step);
#endif // LEAPFROG_CORE_H
