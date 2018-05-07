/*
 * Here also messages that
 * you need show to user,
 * case to fail parse args
 * that this user sent to
 * program.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leapfrog_types.h"
#include "leapfrog_utils.h"
#include "leapfrog_compile.h"
#include "leapfrog_const.h"
#include "leapfrog_memory.h"



#define NEXT_ARG(args, argv) (argv++, args--)
#define CHECK_ARGS(args)     (args - 1 > 0)


__leapfrog_cold__ 
static void show_usage()
{
        printf("\e[1mSYNOPSIS\e[0m\n");
        printf("                \e[1mleapfrog\e[0m -f IN_FILE -t TIME [\e[4mOPTION\e[0m] \e[4mMODE_PRECISION\e[0m\n");
        printf("             or\n");
        printf("                \e[1mleapfrog\e[0m demo DIM COUNT -t TIME [\e[4mOPTION\e[0m] \e[4mMODE_PRECISION\e[0m\n\n");
        printf("For more information and samples type 'leapfrog -h'.\n\n");
        printf("\e[1mAUTHOR\e[0m\n");
        printf("               Written by Daniel Wolkow (2018).\n");
        printf("\e[1mVERSION\e[0m\n"); 
        printf("               Your copy is %s, ", LEAPFROG_VERSION);
        printf("newest version by address \n");
        printf("               \e[4mhttps://github.com/dvolkow/Leapfrog_integration\e[0m\n");
}


__leapfrog_cold__
static void show_version()
{
        printf("Leapfrog N-Body Integrator (dvolkow 2018)\n");
        printf("Version %s, ", LEAPFROG_VERSION);
        printf("newest version by address \e[4mhttps://github.com/dvolkow/Leapfrog_integration\e[0m\n");
}


__leapfrog_cold__ 
static int leapfrog_check_g_state() 
{
        return leapfrog_t_2_double(&g_state.time) > 0 ? LP_SUCCESS
                                                      : LP_BAD_STATE;
}

__leapfrog_cold__
static inline void show_keys() 
{
        printf("\e[1mKEYS\e[0m\n\n");
        printf("         \e[1m-b\e[0m \e[4mBITS\e[0m      set bits precision\n\n");
        printf("         \e[1m-d\e[0m \e[4mDEC\e[0m       set decimal precision\n\n");
        printf("         \e[1m-f\e[0m \e[4mFILE\e[0m      set input data file\n\n");
        printf("         \e[1m-p\e[0m \e[4mDEC\e[0m       set float precision\n\n");
        printf("         \e[1m-t\e[0m \e[4mTIME\e[0m      set time for integrate\n\n");
        printf("         \e[1m-s\e[0m \e[4mFILE\e[0m      set simple symmetry demo mode\n\n");
        printf("         \e[1m-o\e[0m \e[4mFILE\e[0m      set output data file\n\n");
        printf("         \e[1m-h, --help\e[0m\n");
        printf("                      display this help and exit\n\n");
        printf("         \e[1m-v, --version\e[0m\n");
        printf("                      output version information and exit\n\n");
}

__leapfrog_cold__ 
static void show_help()
{
        printf("Leapfrog N-Body Integrator Help you.\n\n\n");
        printf("\e[1mSYNOPSIS\e[0m\n");
        printf("         \e[1mleapfrog\e[0m -f IN_FILE -t TIME [\e[4mOPTION\e[0m] \e[4mMODE_PRECISION\e[0m\n");
        printf("         or\n");
        printf("         \e[1mleapfrog\e[0m demo DIM COUNT -t TIME [\e[4mOPTION\e[0m] \e[4mMODE_PRECISION\e[0m\n\n");
        show_keys();
        printf("\e[1mEXAMPLES\e[0m\n\n   leapfrog demo 2 3 -t 2 -p 0.000001\n\n");
        printf("This run leapfrog for 3 random bodies into 2 dimention space according\n");
        printf("configure file or default configure. Current default configure you can\n");
        printf("get by 'leapfrog -c'. Precision will be 0.000001 part of Hamiltonian of\n");
        printf("generated system.\n\n\n");
        printf("\n   leapfrog -f test.dat -o res.dat -t 2 -d 19 -b 200\n\n");
        printf("This run leapfrog for bodies of test.dat with save results to \n");
        printf("res.dat file, time integate will count to 2, will be use 10^(-19)\n");
        printf("decimal precision as part of Hamiltonian of initial data. Also, \n");
        printf("will extend bitset for float-point operations to 200 bits (instead of\n");
        printf("default 64 bit).\n\n\n");
        
}

/*
 * CLI parser
 *
 * Here we understand how need handle 
 * data. Before this calling you MUST
 * initialize basic structures of core
 * and global space.
 */
__leapfrog_cold__
int leapfrog_parse_arg(int args, char **argv)
{
        double readbuff;

        inline int matches(const char *arg) {
                return !strcmp(*argv, arg);
        }


        while (CHECK_ARGS(args)) {
                NEXT_ARG(args, argv);
                if (matches("-f")) {
                        if (CHECK_ARGS(args)) {
                                NEXT_ARG(args, argv);
                                g_state.input_file = *argv;
                        } else {
                                goto usage_ret;
                        }
                } else if (matches("-o")) {
                        if (CHECK_ARGS(args)) {
                                NEXT_ARG(args, argv);
                                g_state.output_x_file = *argv;
                        } else {
                                goto usage_ret;
                        }
                } else if (matches("-t")) {
                        if (CHECK_ARGS(args)) {
                                NEXT_ARG(args, argv);
                                sscanf(*argv, "%lf", &readbuff);
                                leapfrog_t_set_d(&g_state.time, readbuff);
                                if (!CHECK_ARGS(args)) 
                                        goto ret_ok;
                        } else {
                                goto usage_ret;
                        }
                } else if (matches("-p")) {
                        if (CHECK_ARGS(args)) {
                                NEXT_ARG(args, argv);
                                sscanf(*argv, "%lf", &readbuff);
                                leapfrog_t_set_d(&g_state.precision, readbuff);
                                g_state.precision_type = LP_DEFAULT_ROUNDING;
                                goto ret_ok;
                        } else {
                                goto usage_ret;
                        }
                } else if (matches("-d")) {
                        if (CHECK_ARGS(args)) {
                                NEXT_ARG(args, argv);
                                g_state.precision_rounding = atoi(*argv);
                                g_state.precision_type = LP_DEC_ROUNDING;
                                lp_init_state_precision();
                                goto ret_ok;
                        } else {
                                goto usage_ret;
                        }
                } else if (matches("-b")) {
                        if (CHECK_ARGS(args)) {
                                NEXT_ARG(args, argv);
                                g_state.precision_bits = atoi(*argv);
                                g_state.precision_type = LP_BITS_ROUNDING;
                                if (!(leapfrog_t_2_double(&g_state.precision) > 0)) {
                                        printf("Bad parameters\n");
                                        goto ret_fail;
                                }
                                assert(g_state.precision_bits >= 64);
                                goto ret_ok;
                        } else {
                                goto usage_ret;
                        }
                } else if (matches("demo")) {
                        g_state.isdemo = LP_US_DEMO;
                        if (CHECK_ARGS(args)) {
                                NEXT_ARG(args, argv);
                                g_state.dim = atoi(*argv);
                                if (g_state.dim > LEAPFROG_MAX_DIM) {
                                        printf("%s: bad dim value: %u\n",
#ifdef LEAPFROG_DEBUG
                                                        __FUNCTION__, g_state.dim);
#else
                                                        "LP_PARSER", g_state.dim);
#endif
                                        goto usage_ret;
                                }
                        } else {
                                goto usage_ret;
                        }
                        if (CHECK_ARGS(args)) {
                                NEXT_ARG(args, argv);
                                g_state.count = atoi(*argv);
                                if (g_state.count > LEAPFROG_MAX_COUNT) {
                                        printf("%s: bad count value: %u\n",
#ifdef LEAPFROG_DEBUG
                                                        __FUNCTION__, g_state.count);
#else
                                                        "LP_PARSER", g_state.count);
#endif
                                        goto usage_ret;
                                }
                        } else {
                                goto usage_ret;
                        }
                        if (!CHECK_ARGS(args)) 
                                goto ret_ok;
                } else if (matches("-v") || matches("--version")) {
                        show_version();
                        goto ret_fail;
                } else if (matches("-s")) {
                        g_state.isdemo = LP_SYMDEMO;
                } else if (matches("-h") || matches("--help")) {
                        show_help();
                        goto ret_fail;
                } else {
                        printf("%s: arg \"%s\" not matches\n", 
#ifdef LEAPFROG_DEBUG
                                                        __FUNCTION__, *argv);
#else
                                                        "LP_PARSER", *argv);
#endif
                        goto usage_ret;
                }
        }
usage_ret:
        show_usage();
ret_fail:
        return LP_PARSE_FAIL;
ret_ok:
        return leapfrog_check_g_state();
}
