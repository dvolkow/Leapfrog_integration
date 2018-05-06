#ifndef LEAPFROG_CORE_H
#define LEAPFROG_CORE_H  1

#include "leapfrog_types.h"

void lp_equation_set_ddot_start(equation_t *eq);
void lp_newton_update_eq(equation_t *eq, leapfrog_t *step);

void lp_optimize_step_by_hamiltonian(leapfrog_t *res,
                                     equation_t *eq, 
                                     leapfrog_t *precision,
                                     leapfrog_t *cur_step);

void lp_core_hamiltonian_init(leapfrog_t *); 

void lp_newton_update_kick_drift_kick(equation_t *eq, 
                                        leapfrog_t *step);
void lp_core_structures_init(void);
void lp_core_structures_release(void);
#endif // LEAPFROG_CORE_H
