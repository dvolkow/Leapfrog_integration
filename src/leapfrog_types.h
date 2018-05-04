#ifndef LEAPFROG_TYPES_H
#define LEAPFROG_TYPES_H

#include "leapfrog.h"

#define __leapfrog_t_to_double(value) ((double)value)
#define __double_to_leapfrog_t(value) (value)

typedef struct leafrog_acc_s {
        double v[LEAPFROG_MAX_DIM];
} leafrog_acc_t;

typedef struct double_dot_array_s {
        double a[LEAPFROG_MAX_DIM];
} ddot_array_t;

enum {
        LP_SUCCESS = 0,

        /* must be last */
        LP_ERR_COUNT,
};

#endif // LEAPFROG_TYPES_H
