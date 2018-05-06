#include "leapfrog_types.h"
#include "leapfrog_io.h"
#include "leapfrog_generator.h"

#include <stdio.h>

#define LP_CONFIGURE_PATH 
#define LP_CONFIGURE_FILE  "lp.cfg"

leapfrog_cfg_t g_cfg;

static void leapfrog_init_cfg(void)
{
        g_cfg.DEFAULT_STEP    = LP_DEFAULT_STEP;
        g_cfg.CADR_COUNT      = LEAPFROG_CADR_COUNT;
        g_cfg.GEN_M_SCALE     = LP_GEN_M_SCALE;
        g_cfg.GEN_X_SCALE     = LP_GEN_X_SCALE;
        g_cfg.GEN_XDOT_SCALE  = LP_GEN_XDOT_SCALE;

        sscanf(LP_DEFAULT_FILE_RAND, "%s", g_cfg.DEFAULT_FILE_RAND);
}


void leapfrog_read_cfg(void) 
{
        FILE *cfg_f = lp_fopen(LP_CONFIGURE_FILE);
        if (!cfg_f) {
                printf("No configure file, accept hardcode settings.\n");
                leapfrog_init_cfg();
                return;
        }

        fscanf(cfg_f, "%lf", &g_cfg.DEFAULT_STEP);
        fscanf(cfg_f, "%u", &g_cfg.CADR_COUNT);
        
        fscanf(cfg_f, "%lf", &g_cfg.GEN_M_SCALE);
        fscanf(cfg_f, "%lf", &g_cfg.GEN_X_SCALE);
        fscanf(cfg_f, "%lf", &g_cfg.GEN_XDOT_SCALE);

        fscanf(cfg_f, "%s", g_cfg.DEFAULT_FILE_RAND);

        printf("Configure file accepted.\n");
        lp_fclose(cfg_f);
}
