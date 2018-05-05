#include "leapfrog_compile.h"
#include "leapfrog_core.h"
#include "leapfrog_generator.h"
#include "leapfrog_io.h"
#include "leapfrog_types.h"
#include "leapfrog_const.h"
#include "leapfrog_stats.h"


static equation_t eq;
double step;

void test(const unsigned count) 
{
        unsigned int i;
        equation_t rand_test;
        lp_equation_init_shape(&rand_test, 2, 10);
        lp_equation_init_random(&rand_test);

        FILE *out = fopen("rand.dat", "w+");
        lp_write_eq_to_file(&rand_test, out);
        fprintf(out, "\n\n");

        FORALL_DIM(i, count) {
                lp_newton_update_kick_drift_kick(&rand_test, step);
                LEAPFROG_ASSERT_SAMPLING(i, LEAPFROG_TIME_SCALE) {
                        lp_write_eq_to_file(&rand_test, out);
                        fprintf(out, "\n\n");
                }
        }
        
        lp_fclose(out);
}

int main(int argc, char **argv) 
{
        double t = 0;
        double precision = 0;
        double end_time;
        unsigned i;
        double th_t, th;

        FILE *in = NULL;
        FILE *out = fopen("../data/res.dat", "w+");

        if (argc < 5) {
                printf("%s: few arguments. Usage: ./leapfrog file_in step count precision \n",
                                        __FUNCTION__);
                return LP_FEW_ARG;
        }

        in = lp_fopen(argv[1]);

        lp_init_eq_from_file(&eq, in);
        lp_write_eq_to_screen(&eq);

        sscanf(argv[2], "%lf", &step);
        sscanf(argv[3], "%lf", &end_time);
        sscanf(argv[4], "%lf", &precision);

        th_t = end_time / LEAPFROG_CADR_COUNT;

        lp_write_eq_hamilton_to_screen(&eq);

        lp_equation_init(&eq);
        lp_write_eq_to_file(&eq, out);
        fprintf(out, "\n\n");

        i = 0;
        th = th_t;
        lp_core_hamiltonian_init(lp_hamiltonian(&eq));

        UNTIL_ITS_TIME(t, end_time) {
                step = lp_optimize_step_by_hamiltonian(&eq, precision, step);
                lp_newton_update_kick_drift_kick(&eq, step);
#ifdef LEAPFROG_CONST_STEP
                LEAPFROG_ASSERT_SAMPLING(i, count) {
#else 
                LEAPFROG_ASSERT_SAMPLING(t, th) {
#endif
                        lp_write_eq_to_file(&eq, out);
                        fprintf(out, "\n\n");
                        th += th_t;
                }
                ++i;
                t += step;
        }

        printf("Body after %u steps %lf of integration: \n",
                                                i, step);

        lp_write_eq_to_screen(&eq);
        lp_write_eq_hamilton_to_screen(&eq);

        /*
        printf("Random generated array: \n");
        lp_write_eq_to_screen(&rand_test);
        */

        lp_fclose(out);
        lp_fclose(in);

//        test(count);
        return LP_SUCCESS;
}
