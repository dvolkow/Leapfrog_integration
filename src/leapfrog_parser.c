#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leapfrog_types.h"
#include "leapfrog_utils.h"

extern lp_param_t g_state;

#define NEXT_ARG(args, argv) (argv++, args--)
#define CHECK_ARGS(args)     (args - 1 > 0)

static void show_usage()
{
        printf("Usage: leapfrog    -f IN_FILE -t TIME [-o OUT_FILE]  [-p PREC]\n");
        printf("                                                     [-b PREC_BIT]\n");
        printf("                                                     [-d PREC_DEC]\n");
        printf("                demo DIM COUNT -t TIME [-o OUT_FILE] [-p PREC]\n");
        printf("                                                     [-b PREC_BIT]\n");
        printf("                                                     [-d PREC_DEC]\n");
        printf("                                         \n");
        printf("where PREC is double precision value,\n");
        printf("      PREC_BIT is bits precision length,\n");
        printf("      PREC_DEC is decimal precision length,\n");
        printf("      demo started random generate mode with \n");
        printf("           DIM dimentions for COUNT bodyes \n\n");
        printf("Author: Daniel Wolkow (2018)\n");
}

static int leapfrog_check_g_state() 
{
        return leapfrog_t_2_double(&g_state.time) > 0 ? LP_SUCCESS
                                                      : LP_BAD_STATE;
}

int leapfrog_parse_arg(int args, char **argv)
{
        double readbuff;

        inline int matches(const char *arg) {
                return !strcmp(*argv, arg);
        }

        if (!CHECK_ARGS(args)) {
                goto usage_ret;
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
                                goto ret_ok;
                        } else {
                                goto usage_ret;
                        }
                } else if (matches("-b")) {
                        if (CHECK_ARGS(args)) {
                                NEXT_ARG(args, argv);
                                g_state.precision_bits = atoi(*argv);
                                g_state.precision_type = LP_BITS_ROUNDING;
                                goto ret_ok;
                        } else {
                                goto usage_ret;
                        }
                } else if (matches("demo")) {
                        g_state.isdemo = 1;
                        if (CHECK_ARGS(args)) {
                                NEXT_ARG(args, argv);
                                g_state.dim = atoi(*argv);
                                if (g_state.dim > LEAPFROG_MAX_DIM) {
                                        printf("%s: bad dim value: %u\n",
                                                        __FUNCTION__, g_state.dim);
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
                                                        __FUNCTION__, g_state.count);
                                        goto usage_ret;
                                }
                        } else {
                                goto usage_ret;
                        }
                        if (!CHECK_ARGS(args)) 
                                goto ret_ok;
                } else {
                        printf("%s: arg \"%s\" not matches\n", 
                                                        __FUNCTION__, *argv);
                        goto usage_ret;
                }

        }
usage_ret:
        show_usage();
        return LP_PARSE_FAIL;
ret_ok:
        return leapfrog_check_g_state();
}
