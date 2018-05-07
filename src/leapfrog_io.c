#include <assert.h>

#include "leapfrog_io.h"
#include "leapfrog_generator.h"
#include "leapfrog_compile.h"
#include "leapfrog_stats.h"
#include "leapfrog_utils.h"
#include "leapfrog_memory.h"
#include "leapfrog_precision.h"
#include "leapfrog_math.h"
#include "leapfrog_cfg.h"


__leapfrog_cold__ 
FILE *lp_fopen(const char *fname) 
{
        return fopen(fname, "r+");
}


__leapfrog_cold__ 
void  lp_fclose(FILE *fname) 
{
        fclose(fname);
}



__leapfrog_cold__
int lp_init_eq_from_file(equation_t *eq, FILE *f)
{
        uint16_t i;
        uint8_t  j;
        unsigned int  LP_BODY_COUNT, LP_BODY_DIM;
        double readbuff;
        int err = LP_SUCCESS;

        assert(f);

        /* read dimention and count */
        err = fscanf(f, "%u", &LP_BODY_DIM);
        err = fscanf(f, "%u", &LP_BODY_COUNT);

        GET_DIM(eq)  = LP_BODY_DIM;
        GET_SIZE(eq) = LP_BODY_COUNT;
        
        assert(GET_DIM(eq) <= LEAPFROG_MAX_DIM);
        assert(GET_SIZE(eq) <= LEAPFROG_MAX_COUNT);

        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        err = fscanf(f, "%lf", &readbuff);
                        leapfrog_t_set_d(&GET_X(eq, i, j), readbuff);
                }
                

                FORALL_DIM(j, GET_DIM(eq)) {
                        err = fscanf(f, "%lf", &readbuff);
                        leapfrog_t_set_d(&GET_X_DOT(eq, i, j), readbuff);
                }

                err = fscanf(f, "%lf", &readbuff);
                leapfrog_t_set_d(&GET_M(eq, i), readbuff);
                GET_ID(eq, i) = i;
        }
        return err;
}



__leapfrog_cold__ __leapfrog_const__
void lp_write_eq_to_screen(equation_t *eq) 
{
        uint16_t i;
        uint8_t  j;

        FORALL_BODY(i, GET_SIZE(eq)) {
                printf("Body #: %u\n", i);
                printf("x:\n   ");
                FORALL_DIM(j, GET_DIM(eq)) {
                        printf("%lf ", leapfrog_t_2_double(&GET_X(eq, i, j)));
                }
                printf("\nx_dot:\n   ");
                FORALL_DIM(j, GET_DIM(eq)) {
                        printf("%lf ", leapfrog_t_2_double(&GET_X_DOT(eq, i, j)));
                }
                printf("\nM:    %lf\n", leapfrog_t_2_double(&GET_M(eq, i)));
        }
        
}



__leapfrog_cold__ __leapfrog_const__
void lp_write_eq_hamilton_to_screen(equation_t *eq) 
{
        leapfrog_t h;

        LP_T_INIT(h);
        leapfrog_t_set_d(&h, 0);
        lp_hamiltonian(&h, eq);


        printf("Hamiltonian: %lf\n", leapfrog_t_2_double(&h));
        LP_T_RELEASE(h);
}



__leapfrog_hot__ __leapfrog_pure__
void lp_write_eq_x_to_file(equation_t *eq, FILE *f) 
{
        uint16_t i;
        uint8_t  j;

        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        fprintf(f, "%lf ", leapfrog_t_2_double(&GET_X(eq, i, j)));
                }
                fprintf(f, "%f", leapfrog_t_2_double(&GET_M(eq, i)));
                fprintf(f, "\n");
        }
}




__leapfrog_cold__ __leapfrog_pure__
void lp_write_eq_to_file(equation_t *eq, FILE *f) 
{
        uint16_t i;
        uint8_t  j;

        assert(f);
        assert(eq);

        fprintf(f, "%u %u\n", GET_DIM(eq), GET_SIZE(eq));

        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        fprintf(f, "%lf ", leapfrog_t_2_double(&GET_X(eq, i, j)));
                }
                fprintf(f, "\n");
                FORALL_DIM(j, GET_DIM(eq)) {
                        fprintf(f, "%lf ", leapfrog_t_2_double(&GET_X_DOT(eq, i, j)));
                }
                fprintf(f, "\n");
                fprintf(f, "%f\n", leapfrog_t_2_double(&GET_M(eq, i)));
        }
}



__leapfrog_cold__ __leapfrog_pure__
void lp_rbody_dump(equation_t *eq) 
{
#ifdef LEAPFROG_DEBUG
        assert(g_cfg.DEFAULT_FILE_RAND);
#endif
        FILE *f = fopen(g_cfg.DEFAULT_FILE_RAND, "w+");
#ifdef LEAPFROG_DEBUG
        assert(f);
#endif
        lp_write_eq_to_file(eq, f);
        lp_fclose(f);
        printf("dumped to %s!\n", g_cfg.DEFAULT_FILE_RAND);
}
