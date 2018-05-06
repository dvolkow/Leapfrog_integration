#include <time.h>
#include <math.h>
#include <stdlib.h>

#ifdef LEAPFROG_DEBUG
        #include <assert.h>
#endif

#include "leapfrog_const.h"
#include "leapfrog_types.h"
#include "leapfrog_compile.h"
#include "leapfrog_generator.h"
#include "leapfrog_utils.h"
#include "leapfrog_memory.h"
#include "leapfrog_math.h"
#include "leapfrog_cfg.h"


__leapfrog_cold__
void lp_equation_init_m_random(equation_t *eq)
{        
        uint16_t i;
        srand(time(NULL));

        FORALL_BODY(i, GET_SIZE(eq)) {
                leapfrog_t_set_d(&GET_M(eq, i), g_cfg.GEN_M_SCALE * fabs(((double)rand() / RAND_MAX * 2.0 - 1.0)));
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
                        leapfrog_t_set_d(&GET_X_DOT(eq, i, j), g_cfg.GEN_XDOT_SCALE * ((double)rand() / RAND_MAX * 2.0 - 1.0));
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
                        leapfrog_t_set_d(&GET_X(eq, i, j), g_cfg.GEN_X_SCALE * ((double)rand() / RAND_MAX * 2.0 - 1.0));
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

__leapfrog_cold__
void lp_equation_init(equation_t *eq) 
{
        uint16_t i;
        uint8_t  j;
        FORALL_BODY(i, LEAPFROG_MAX_COUNT) {
                FORALL_DIM(j, LEAPFROG_MAX_DIM) {
                        LP_T_INIT(GET_X(eq, i, j));
                        LP_T_INIT(GET_X_DOT(eq, i, j));
                        LP_T_INIT(GET_X_DDOT(eq, i, j));
                }
                LP_T_INIT(GET_M(eq, i));
        }
}


__leapfrog_cold__
void lp_equation_release(equation_t *eq) 
{
        uint16_t i;
        uint8_t  j;
        FORALL_BODY(i, LEAPFROG_MAX_COUNT) {
                FORALL_DIM(j, LEAPFROG_MAX_DIM) {
                        LP_T_RELEASE(GET_X(eq, i, j));
                        LP_T_RELEASE(GET_X_DOT(eq, i, j));
                        LP_T_RELEASE(GET_X_DDOT(eq, i, j));
                }
                LP_T_RELEASE(GET_M(eq, i));
        }
}

__leapfrog_cold__ 
void lp_param_t_init(lp_param_t *state) 
{
        state->dim   = LEAPFROG_MAX_DIM;
        state->count = LEAPFROG_MAX_COUNT;

        LP_T_INIT(state->counter);
        LP_T_INIT(state->time);
        LP_T_INIT(state->step);
        LP_T_INIT(state->precision);

        lp_equation_init(&state->eq);

#ifdef LEAPFROG_DEBUG
        assert(g_cfg.DEFAULT_STEP > 0);
#endif
        leapfrog_t_set_d(&state->step, g_cfg.DEFAULT_STEP);
        state->isdemo = 0;
        state->precision_type = LP_DEFAULT_ROUNDING;
}

__leapfrog_cold__
void lp_param_t_release(lp_param_t *state)
{
        lp_equation_release(&state->eq);
        LP_T_RELEASE(state->counter);
        LP_T_RELEASE(state->time);
        LP_T_RELEASE(state->step);
        LP_T_RELEASE(state->precision);
}
