#ifndef LEAPFROG_IO_H 
#define LEAPFROG_IO_H  1

#include <stdio.h>
#include "leapfrog_types.h"

/*
 * INITIALIZE BEFORE USED!
 */
void lp_io_init();
void lp_io_release();

FILE *lp_fopen(const char *);
void  lp_fclose(FILE *);

void lp_fprint_ddot_array(ddot_array_t *, FILE *);

/*
 * Read file with initial data
 *
 * Format:
 *        DIMENTION  COUNT_OF_BODY(aka N)
 *    |    x_0      x_1     ... x_N
 *  N |    x_dot_0  x_dot_1 ... x_dot_N
 *    |    m
 *
 * Also, see "sample" directory.
 */
int lp_init_eq_from_file(equation_t *eq, FILE *);


/*
 * Write coords of bodies to file.
 *
 * Format:
 *       x_0 x_1 ... x_N
 */       
void lp_write_eq_x_to_file(equation_t *eq, FILE *);

void lp_write_eq_to_file(equation_t *eq, FILE *);

void lp_rbody_dump(equation_t *eq);

void lp_write_eq_to_screen(equation_t *eq); 
void lp_write_eq_hamilton_to_screen(equation_t *eq); 

void lp_eq_hamiltonian_dump(equation_t *eq);

#endif // LEAPFROG_IO_H
