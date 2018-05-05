#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "leapfrog_types.h"
#include "leapfrog_compile.h"
#include "leapfrog_generator.h"


__leapfrog_cold__
void lp_equation_init_m_random(equation_t *eq)
{        
        uint16_t i;
        srand(time(NULL));

        FORALL_BODY(i, GET_SIZE(eq)) {
                GET_M(eq, i) = LP_GEN_M_SCALE * fabs(((double)rand() / RAND_MAX * 2.0 - 1.0));
        }
}


__leapfrog_cold__
void lp_equation_init_xdot_random(equation_t *eq) 
{
        uint16_t i;
        uint8_t  j;
        srand(time(NULL));

        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        GET_X_DOT(eq, i, j) = LP_GEN_XDOT_SCALE * ((double)rand() / RAND_MAX * 2.0 - 1.0);
                }
        }
}


__leapfrog_cold__
void lp_equation_init_x_random(equation_t *eq) 
{
        uint16_t i;
        uint8_t  j;
        srand(time(NULL));

        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        GET_X(eq, i, j) = LP_GEN_X_SCALE * ((double)rand() / RAND_MAX * 2.0 - 1.0);
                }
        }
}

__leapfrog_cold__
void lp_equation_init_random(equation_t *eq)
{
        lp_equation_init_x_random(eq);
        lp_equation_init_xdot_random(eq);
        lp_equation_init_m_random(eq);
}

__leapfrog_cold__
void lp_equation_init_shape(equation_t *eq, 
                            const uint8_t dim,
                            const uint16_t size)
{
        GET_DIM(eq) = dim;
        GET_SIZE(eq) = size;
}
