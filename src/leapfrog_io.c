#include <assert.h>

#include "leapfrog_io.h"
#include "leapfrog_compile.h"
#include "leapfrog_stats.h"


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
void lp_init_eq_from_file(equation_t *eq, FILE *f)
{
        uint16_t i;
        uint8_t  j;
        unsigned int  LP_BODY_COUNT, LP_BODY_DIM;

        /* read dimention and count */
        fscanf(f, "%u", &LP_BODY_DIM);
        fscanf(f, "%u", &LP_BODY_COUNT);

        GET_DIM(eq)  = LP_BODY_DIM;
        GET_SIZE(eq) = LP_BODY_COUNT;
        
        assert(GET_DIM(eq) <= LEAPFROG_MAX_DIM);
        assert(GET_SIZE(eq) <= LEAPFROG_MAX_COUNT);
        // printf("%s: OK line %d\n", __FUNCTION__, __LINE__);
        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) 
                        fscanf(f, "%lf", &GET_X(eq, i, j));
                

                FORALL_DIM(j, GET_DIM(eq)) 
                        fscanf(f, "%lf", &GET_X_DOT(eq, i, j));

                fscanf(f, "%lf", &GET_M(eq, i));
                GET_ID(eq, i) = i;
        }
                
}

__leapfrog_cold__
void lp_write_eq_to_screen(const equation_t *eq) 
{
        uint16_t i;
        uint8_t  j;

        FORALL_BODY(i, GET_SIZE(eq)) {
                printf("Body #: %u\n", i);
                printf("x:\n   ");
                FORALL_DIM(j, GET_DIM(eq)) {
                        printf("%lf ", GET_X(eq, i, j));
                }
                printf("\nx_dot:\n   ");
                FORALL_DIM(j, GET_DIM(eq)) {
                        printf("%lf ", GET_X_DOT(eq, i, j));
                }
                printf("\nM:    %lf\n", GET_M(eq, i));
        }
        
}

__leapfrog_cold__
void lp_write_eq_hamilton_to_screen(const equation_t *eq) 
{
        printf("Hamiltonian: %lf\n", lp_hamiltonian(eq));
}


__leapfrog_hot__
void lp_write_eq_to_file(const equation_t *eq, FILE *f) 
{
        uint16_t i;
        uint8_t  j;

        FORALL_BODY(i, GET_SIZE(eq)) {
                FORALL_DIM(j, GET_DIM(eq)) {
                        fprintf(f, "%lf ", GET_X(eq, i, j));
                }
                fprintf(f, "%f", GET_M(eq, i));
                fprintf(f, "\n");
        }
}
