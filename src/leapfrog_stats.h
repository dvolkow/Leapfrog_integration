/*
 * Here calculate basic statistic
 * of global structures. Most of
 * them is Hamiltonial, that use
 * for balance integrate step.
 * Also may be include another 
 * features for monitoring of sta-
 * tements.
 */
#ifndef LEAPFROG_STATS_H
#define LEAPFROG_STATS_H 1

#include "leapfrog_types.h"


void lp_hamiltonian(leapfrog_t *res, equation_t *);


void lp_stats_init(void);
void lp_stats_release(void); 
#endif // LEAPFROG_STATS_H
