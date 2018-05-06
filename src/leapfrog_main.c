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


static equation_t eq;
leapfrog_t step;

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
                lp_newton_update_kick_drift_kick(&rand_test, &step);
#ifdef LEAPFROG_CONST_STEP
                LEAPFROG_ASSERT_SAMPLING(i, LEAPFROG_TIME_SCALE) {

                        lp_write_eq_to_file(&rand_test, out);
                        fprintf(out, "\n\n");
                }
#endif
        }
        
        lp_fclose(out);
}

int main(int argc, char **argv) 
{
        leapfrog_t t, h_;
        leapfrog_t precision;
        leapfrog_t end_time;
        leapfrog_t th_t, th, tmp;

        double buff; 
        unsigned i;

        LP_T_INIT(t);
        LP_T_INIT(h_);
        LP_T_INIT(precision);
        LP_T_INIT(end_time);
        LP_T_INIT(th_t);
        LP_T_INIT(th);
        LP_T_INIT(tmp);
        LP_T_INIT(step);

        lp_core_structures_init();
        printf("Core structures initial success\n");
        leapfrog_t_set_d(&t, 0);

        printf("%s: line %d success\n", __FUNCTION__, __LINE__);


        FILE *in = NULL;
        FILE *out = fopen("../data/res.dat", "w+");

        if (argc < 5) {
                printf("%s: few arguments. Usage: ./leapfrog file_in step count precision \n",
                                        __FUNCTION__);
                return LP_FEW_ARG;
        }

        in = lp_fopen(argv[1]);

        lp_equation_init(&eq);

        lp_init_eq_from_file(&eq, in);
        lp_write_eq_to_screen(&eq);

        sscanf(argv[2], "%lf", &buff);
        leapfrog_t_set_d(&step, buff);
        sscanf(argv[3], "%lf", &buff);
        leapfrog_t_set_d(&end_time, buff);
        sscanf(argv[4], "%lf", &buff);
        leapfrog_t_set_d(&precision, buff);

        leapfrog_t_set_i(&tmp, LEAPFROG_CADR_COUNT);
        leapfrog_div(&th_t, &end_time, &tmp);

        lp_write_eq_hamilton_to_screen(&eq);

        lp_equation_set_ddot_start(&eq);
        lp_write_eq_to_file(&eq, out);
        fprintf(out, "\n\n");

        i = 0;
        leapfrog_t_set_lp(&th, &th_t);
        
        lp_hamiltonian(&h_, &eq);
        lp_core_hamiltonian_init(&h_);

        UNTIL_ITS_TIME(&t, &end_time) {
                lp_optimize_step_by_hamiltonian(&step, &eq, &precision, &step);
                lp_newton_update_kick_drift_kick(&eq, &step);
#ifdef LEAPFROG_CONST_STEP
                LEAPFROG_ASSERT_SAMPLING(i, count) {
#else 
                LEAPFROG_ASSERT_SAMPLING(&t, &th) {
#endif
                        lp_write_eq_to_file(&eq, out);
                        fprintf(out, "\n\n");
                        leapfrog_sum(&th, &th, &th_t);
                }
                ++i;
                leapfrog_sum(&t, &t, &step);
        }

        printf("Body after %u steps %lf of integration: \n",
                                                i, leapfrog_t_2_double(&step));

        lp_write_eq_to_screen(&eq);
        lp_write_eq_hamilton_to_screen(&eq);

        //
        //printf("Random generated array: \n");
        //lp_write_eq_to_screen(&rand_test);
        //

        lp_fclose(out);
        lp_fclose(in);

        LP_T_RELEASE(h_);
        LP_T_RELEASE(step);
//        test(count);
        lp_equation_release(&eq);
        LP_T_RELEASE(t);
        LP_T_RELEASE(precision);
        LP_T_RELEASE(end_time);
        LP_T_RELEASE(th_t);
        LP_T_RELEASE(th);
        LP_T_RELEASE(tmp);


        return LP_SUCCESS;
}
