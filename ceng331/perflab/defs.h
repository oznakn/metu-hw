/*
 * driver.h - Various definitions for the Performance Lab.
 * 
 * DO NOT MODIFY ANYTHING IN THIS FILE
 */
#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdlib.h>

#define RIDX(i,j,n) ((i)*(n)+(j))

typedef struct {
  char *team;
  char *name1, *id1;
  char *name2, *id2;
  char *name3, *id3;
} team_t;

extern team_t team;

typedef struct {
   unsigned short red;
   unsigned short green;
   unsigned short blue;
} pixel;

typedef void (*lab_test_func1) (int, pixel*, short*, pixel*);
typedef void (*lab_test_func2) (int, int*, int*, int*);

void bokeh(int, pixel *, short *, pixel *);
void hadamard(int, int *, int *, int *);

void register_bokeh_functions(void);
void register_hadamard_functions(void);
void add_bokeh_function(lab_test_func1, char*);
void add_hadamard_function(lab_test_func2, char*);

#endif /* _DEFS_H_ */

