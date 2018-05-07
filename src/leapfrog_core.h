#ifndef LEAPFROG_CORE_H
#define LEAPFROG_CORE_H  1

#include "leapfrog_types.h"


void lp_core_hamiltonian_init(leapfrog_t *); 
void lp_equation_set_ddot_start(equation_t *eq);

void lp_core_structures_init(void);
void lp_core_structures_release(void);

void lp_core_up(void);

#endif // LEAPFROG_CORE_H
