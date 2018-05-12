#include "leapfrog_compile.h"
#include "leapfrog_core.h"
#include "leapfrog_generator.h"
#include "leapfrog_io.h"
#include "leapfrog_types.h"
#include "leapfrog_const.h"
#include "leapfrog_stats.h"
#include "leapfrog_utils.h"
#include "leapfrog_math.h"
#include "leapfrog_memory.h"
#include "leapfrog_parser.h"
#include "leapfrog_cfg.h"
#include "leapfrog_time.h"


extern lp_param_t g_state;

/* Accumutalors variable */
leapfrog_t t, thr, delta;




/*
 * ------------ATTENTION!!------------
 * FIRST CALLED FUNCTION BEFORE MAIN ENRTY!!! 
 */
__leapfrog_cold__
static inline void LEAPFROG_PRECISION_INITIALIZE(void)
{
        g_state.precision_bits = LP_PRECISION_BITS_DEFAULT;
}




/*
 * Run basic operations cycle here.
 * Are global states and variables 
 * MUST BE INITIALIZED when run it!
 */
__leapfrog_cold__
static void leapfrog_run() 
{
        unsigned i;

        FILE *in = lp_fopen(g_state.input_file);
#ifdef LEAPFROG_DEBUG
        printf("Open %s!\n", g_state.input_file);
#endif
        lp_equation_init(&g_state.eq);
        lp_init_eq_from_file(&g_state.eq, in);

        lp_write_eq_hamilton_to_screen(&g_state.eq);
        lp_equation_set_ddot_start(&g_state.eq);

        lp_fclose(in);

        FILE *out_x = fopen(
                        g_state.output_x_file ? g_state.output_x_file 
                                              : "leapfrog_out_x.dat", 
                                                                 "w+");

        lp_write_eq_x_to_file(&g_state.eq, out_x);
        fprintf(out_x, "\n\n");

#ifdef LEAPFROG_DEBUG
        printf("DEFAULT_FILE_RAND %s!\n", g_cfg.DEFAULT_FILE_RAND);
        printf("g_cfg.output_x_file %s!\n", g_state.output_x_file);
#endif
        i = 0;
        UNTIL_ITS_TIME(&t, &g_state.time) {
                lp_core_up();
                LEAPFROG_ASSERT_SAMPLING(&t, &thr) {
                        lp_write_eq_x_to_file(&g_state.eq, out_x);
                        fprintf(out_x, "\n\n");
                        if (g_state.output_h_file)
                                lp_eq_hamiltonian_dump(&g_state.eq);

                        printf("\r%0.2f%% ready. ", 
                                  100 * leapfrog_t_2_double(&t) / leapfrog_t_2_double(&g_state.time));
                        print_estimated_time(&t);
                        print_elapsed_time();
                        leapfrog_sum(&thr, &thr, &delta);
                }
                ++i;
                leapfrog_sum(&t, &t, &g_state.step);
        }

        printf("\rBody after %u steps (final size %lf) of integration: \n",
                                                i, leapfrog_t_2_double(&g_state.step));
        lp_write_eq_hamilton_to_screen(&g_state.eq);

        lp_fclose(out_x);
}



/*
 * DEMO: generate rangom array by defined shape
 *       write it into file and go to leapfrog_run
 */
__leapfrog_cold__
static void demo_run() 
{
        printf("DEMO run..\n");
        printf("Parameters: %u dimensions, %u bodies.\n", 
                                g_state.dim, g_state.count);
        printf("Precision: %d type, %lf value.\n", 
                                g_state.precision_type, 
                                 leapfrog_t_2_double(&g_state.precision));

        lp_equation_init_shape(&g_state.eq, g_state.dim, g_state.count);

        switch(g_state.isdemo) {
                case(LP_US_DEMO):
                        lp_equation_init_random(&g_state.eq);
                        break;
                case(LP_SYMDEMO):
                        lp_generate_symmetry(&g_state.eq);
                        break;
        }


#ifdef LEAPFROG_DEBUG
        printf("%s: lp_rbody_dump ...\n", __FUNCTION__);
#endif
        
        FILE *out = fopen(g_cfg.DEFAULT_FILE_RAND, "w+");
        lp_write_eq_to_file(&g_state.eq, out);
        lp_fclose(out);

#ifdef LEAPFROG_DEBUG
        printf("%s: lp_rbody_dump LP_SUCCESS\n", __FUNCTION__);
#endif
        g_state.input_file = g_cfg.DEFAULT_FILE_RAND;
        leapfrog_run();
}



/*
 * ------------ATTENTION!!-------------
 *  This must be as possible near begin
 */
__leapfrog_cold__
static void leapfrog_main_global_init() 
{
        LP_T_INIT(t);
        LP_T_INIT(thr);
        LP_T_INIT(delta);

        leapfrog_t_set_i(&t, g_cfg.CADR_COUNT);
        leapfrog_div(&delta, &g_state.time, &t);

        lp_hamiltonian(&t, &g_state.eq);
        lp_core_hamiltonian_init(&t);

        leapfrog_t_set_lp(&thr, &t);
        leapfrog_t_set_i(&t, 0);

        lp_time_start_init();
}


__leapfrog_cold__
static void leapfrog_main_global_release() 
{
        LP_T_RELEASE(delta);
        LP_T_RELEASE(thr);
        LP_T_RELEASE(t);
}



int main(int argc, char **argv) 
{
        
        int err = LP_SUCCESS;
        /* !!!! */
        LEAPFROG_PRECISION_INITIALIZE();

        leapfrog_read_cfg();
        lp_core_structures_init();
#ifdef LEAPFROG_DEBUG
        printf("Core structures initialized success\n");
#endif


        err = leapfrog_parse_arg(argc, argv);
        if (err)
                return err;

        leapfrog_main_global_init();
        lp_stats_init();
        lp_io_init();
#ifdef LEAPFROG_DEBUG
        printf("IO structures initialized success\n");
#endif

        if (g_state.isdemo) 
                demo_run();
        else 
                leapfrog_run();

        lp_io_release();
        lp_stats_release();
        lp_core_structures_release();
        leapfrog_main_global_release();

        return LP_SUCCESS;
}
