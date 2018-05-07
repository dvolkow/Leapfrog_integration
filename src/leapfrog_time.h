/*
 * Print into console 
 * timings for feedback
 * and estimations wait
 * length.
 */
#ifndef LEAPFROG_TIME_H
#define LEAPFROG_TIME_H  1

#include "leapfrog_types.h"

void lp_time_start_init(void);
void print_estimated_time(leapfrog_t *);
void print_elapsed_time(void);

#endif  // LEAPFROG_TIME_H
