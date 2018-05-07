/*
 * Global configure setting
 * must be singleton. We into
 * C, because it is simulation.
 *              Please,
 * NOT INCLUDE "leapfrog_cfg.h" HERE!!!
 * Format of configure file 
 * see in header.
 */
#include "leapfrog_types.h"
#include "leapfrog_compile.h"
#include "leapfrog_io.h"
#include "leapfrog_generator.h"

#include <stdio.h>

#define LP_CONFIGURE_PATH 
#define LP_CONFIGURE_FILE  "lp.cfg"

/*
 * Instanse of global configure
 * MUST BE ONCE
 */
leapfrog_cfg_t  g_cfg;

/*
 * Instanse of global statement
 * MUST BE ONCE
 */
lp_param_t      g_state;



__leapfrog_cold__
static inline void leapfrog_init_cfg(void)
{
        g_cfg.DEFAULT_STEP    = LP_DEFAULT_STEP;
        g_cfg.CADR_COUNT      = LEAPFROG_CADR_COUNT;
        g_cfg.GEN_M_SCALE     = LP_GEN_M_SCALE;
        g_cfg.GEN_X_SCALE     = LP_GEN_X_SCALE;
        g_cfg.GEN_XDOT_SCALE  = LP_GEN_XDOT_SCALE;

        sscanf(LP_DEFAULT_FILE_RAND, "%s", g_cfg.DEFAULT_FILE_RAND);
}



__leapfrog_cold__
int leapfrog_read_cfg(void) 
{
        int err = LP_SUCCESS;
        FILE *cfg_f = lp_fopen(LP_CONFIGURE_FILE);
        if (!cfg_f) {
                printf("No configure file, accept hardcode settings.\n");
                leapfrog_init_cfg();
                return err;
        }

        err = fscanf(cfg_f, "%lf", &g_cfg.DEFAULT_STEP);
        err = fscanf(cfg_f, "%u", &g_cfg.CADR_COUNT);
        
        err = fscanf(cfg_f, "%lf", &g_cfg.GEN_M_SCALE);
        err = fscanf(cfg_f, "%lf", &g_cfg.GEN_X_SCALE);
        err = fscanf(cfg_f, "%lf", &g_cfg.GEN_XDOT_SCALE);

        err = fscanf(cfg_f, "%s", g_cfg.DEFAULT_FILE_RAND);

        printf("Configure file accepted.\n");
        lp_fclose(cfg_f);
        return err;
}
