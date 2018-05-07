#ifndef LEAPFROG_CFG_H
#define LEAPFROG_CFG_H  1

#include "leapfrog_types.h"

/*
 * FORMAT CONFIGURE FILE:
 *
 *
 * LP_DEFAULT_STEP
 * LEAPFROG_CADR_COUNT
 * LP_GEN_M_SCALE
 * LP_GEN_X_SCALE
 * LP_GEN_XDOT_SCALE
 * LP_DEFAULT_FILE_RAND
 */

extern leapfrog_cfg_t g_cfg;

void leapfrog_read_cfg(void); 
#endif //  LEAPFROG_CFG_H  

