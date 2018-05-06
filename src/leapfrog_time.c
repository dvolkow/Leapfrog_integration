#include <time.h>
#include <stdio.h>
#include "leapfrog_types.h"
#include "leapfrog_utils.h"

#define  __LP_MIN       60
#define  __LP_HOUR      60

extern lp_param_t g_state;
static time_t start_time;

void lp_time_start_init(void)
{
        start_time = time(NULL);
}

void print_elapsed_time(void)
{
        time_t current_time = time(NULL);
        unsigned int h, m;
        double seconds = difftime(current_time, start_time);

        h = (unsigned int)(seconds/(__LP_MIN * __LP_HOUR));
        seconds -= (__LP_MIN * __LP_HOUR) * h;
        m = (unsigned int)(seconds / __LP_MIN);
        seconds -= __LP_MIN * m;

        printf("Elapsed time: %u h : %u m : %0.1f s. ", h, m, seconds);

}

void print_estimation_time(leapfrog_t *t)
{
        time_t current_time = time(NULL);
        unsigned int h, m;
        double seconds = difftime(current_time, start_time);
        seconds = leapfrog_t_2_double(&g_state.time) / 
                        leapfrog_t_2_double(t) * seconds - seconds;


        h = (unsigned int)(seconds/(__LP_MIN * __LP_HOUR));
        seconds -= (__LP_MIN * __LP_HOUR) * h;
        m = (unsigned int)(seconds / __LP_MIN);
        seconds -= __LP_MIN * m;

        printf("Estimation time: %u h : %u m : %0.1f s", h, m, seconds);
}

