#ifndef LEAPFROG_H
#define LEAPFROG_H   1

#include <stdio.h>
#include <stdint.h>

#define LEAPFROG_MAX_COUNT 1000
#define LEAPFROG_MAX_DIM   3

#define LEAPFROG_STEP      1

typedef double   leapfrog_t;
typedef uint16_t point_id_t;


typedef struct point_s {
        leapfrog_t  x[LEAPFROG_MAX_DIM];
        leapfrog_t  x_dot[LEAPFROG_MAX_DIM];
        leapfrog_t  m;
        point_id_t  id;
        double      x_ddot_prev[LEAPFROG_MAX_DIM];     
} point_t;


typedef struct equation_s {
        point_t     body[LEAPFROG_MAX_COUNT];
} equation_t;

#endif // LEAPFROG_H
