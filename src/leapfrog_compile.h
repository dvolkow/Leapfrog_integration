#ifndef LEAPFROG_COMPILE_H
#define LEAPFROG_COMPILE_H  1

#include<assert.h>
//#include<linux/cache.h>
//#include<linux/compiler.h>

#define FORALL_BODY(i, lim) for (i = 0; i < (lim); ++i)
#define FORALL_DIM(j, lim)  for (j = 0; j < (lim); ++j)

#define UNTIL_ITS_TIME(current_time, estimated_time) while (leapfrog_cmp(current_time, estimated_time) < 0)

#ifdef LEAPFROG_CONST_STEP
        #define LEAPFROG_ASSERT_SAMPLING(i, freq) if ((i) % (freq) == 0) 
#else
        #define LEAPFROG_ASSERT_SAMPLING(t, thr) if (leapfrog_cmp(t, thr) > 0) 
#endif


#define __leapfrog_hot__        __attribute__((__hot__))
#define __leapfrog_cold__       __attribute__((__hot__))
#define __leapfrog_unused__     __attribute__((unused))
#define __leapfrog_pure__       __attribute_pure__
#define __leapfrog_const__      __attribute_const__

#endif  // LEAPFROG_COMPILE_H  
