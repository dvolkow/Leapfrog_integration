#ifndef LEAPFROG_TYPES_H
#define LEAPFROG_TYPES_H  1

#include <stdint.h>
#include "leapfrog_const.h"

#ifndef LP_MPFR_CASE
        typedef double   leapfrog_t;
#else
        #include <mpfr.h>
        typedef mpfr_t   leapfrog_t;
#endif

typedef uint16_t point_id_t;

typedef struct double_dot_array_s {
        leapfrog_t a[LEAPFROG_MAX_DIM];
} ddot_array_t;

#define GET_DDOT_A(array, i) ((array)->a[i])


typedef struct point_s {
        leapfrog_t  x[LEAPFROG_MAX_DIM];
        leapfrog_t  x_dot[LEAPFROG_MAX_DIM];
        leapfrog_t  x_ddot_prev[LEAPFROG_MAX_DIM];     

        leapfrog_t  m;
        point_id_t  id;
} point_t;


typedef struct equation_s {
        point_t     body[LEAPFROG_MAX_COUNT];
        point_id_t  size;
        uint8_t     dim;
} equation_t;


#define GET_BODY(eq, i)         ((eq)->body[i])
#define GET_X(eq, i, j)         ((eq)->body[i].x[j])
#define GET_X_DOT(eq, i, j)     ((eq)->body[i].x_dot[j])
#define GET_X_DDOT(eq, i, j)    ((eq)->body[i].x_ddot_prev[j])
#define GET_M(eq, i)            ((eq)->body[i].m)
#define GET_ID(eq, i)           ((eq)->body[i].id)
#define GET_DIM(eq)             ((eq)->dim)
#define GET_SIZE(eq)            ((eq)->size)


typedef struct leapfrog_parameters_s {
        char *input_file;
        char *output_x_file;
        char *output_xdot_file;
        char *output_h_file;

        unsigned int dim;
        unsigned int count;

        leapfrog_t counter;
        leapfrog_t time;
        leapfrog_t step;

        union {
                leapfrog_t precision;
                int  precision_bits;
                int  precision_rounding;
        };

        char isdemo;
        char precision_type;

        equation_t eq;
} lp_param_t;


/*
 * Leapfrog configure arrtibutes
 */
typedef struct leapfrog_cfg_s {
        double       DEFAULT_STEP;
        unsigned int CADR_COUNT;

        double       GEN_M_SCALE;
        double       GEN_X_SCALE;
        double       GEN_XDOT_SCALE;

        char         DEFAULT_FILE_RAND[LP_NAME_LENGTH];
} leapfrog_cfg_t;

/*
 * Precision types
 */
enum {
        LP_DEFAULT_ROUNDING = 0,
        LP_BITS_ROUNDING,
        LP_DEC_ROUNDING,
};

/*
 * Fields int point_s
 */
enum {
        LP_X = 0,
        LP_XDOT,
        LP_XDDOT,
        LP_M,
        LP_ID,
};

/*
 * Error types
 */
enum {
        LP_SUCCESS = 0,
        LP_FEW_ARG,
        LP_PARSE_FAIL,
        LP_BAD_STATE,

        /* must be last */
        LP_ERR_COUNT,
};

#endif // LEAPFROG_TYPES_H
