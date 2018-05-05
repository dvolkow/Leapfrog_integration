#ifndef LEAPFROG_GENERATOR_H
#define LEAPFROG_GENERATOR_H  1

#include "leapfrog_types.h"


#define LP_GEN_M_SCALE          2
#define LP_GEN_X_SCALE          90
#define LP_GEN_XDOT_SCALE       0.5


void lp_equation_init_x_random(equation_t *eq);
void lp_equation_init_xdot_random(equation_t *eq);
void lp_equation_init_m_random(equation_t *eq);

void lp_equation_init_shape(equation_t *eq, 
                            const uint8_t dim,
                            const uint16_t size);

void lp_equation_init_random(equation_t *eq);



#endif // LEAPFROG_GENERATOR_H
