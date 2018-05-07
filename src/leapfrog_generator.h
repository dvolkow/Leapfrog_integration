#ifndef LEAPFROG_GENERATOR_H
#define LEAPFROG_GENERATOR_H  1

#include "leapfrog_types.h"


#define LP_GEN_M_SCALE          3.5
#define LP_GEN_X_SCALE          50
#define LP_GEN_XDOT_SCALE       0.1

#define LP_DEFAULT_FILE_RAND    "leapfrog_random.dat"

void lp_equation_init_x_random(equation_t *eq);
void lp_equation_init_xdot_random(equation_t *eq);
void lp_equation_init_m_random(equation_t *eq);

void lp_equation_init_shape(equation_t *eq, 
                            const uint8_t dim,
                            const uint16_t size);

void lp_equation_init_random(equation_t *eq);


void lp_equation_init(equation_t *eq);
void lp_equation_release(equation_t *eq);

void lp_param_t_init(lp_param_t *state);
void lp_param_t_release(lp_param_t *state);

void lp_generate_symmetry(equation_t *eq);
#endif // LEAPFROG_GENERATOR_H
