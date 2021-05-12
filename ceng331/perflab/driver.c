/*******************************************************************
 * 
 * driver.c - Driver program for CS:APP Performance Lab
 * 
 * In kernels.c, students generate an arbitrary number of bokeh and
 * hadamard test functions, which they then register with the driver
 * program using the add_bokeh_function() and add_hadamard_function()
 * functions.
 * 
 * The driver program runs and measures the registered test functions
 * and reports their performance.
 * 
 * Copyright (c) 2002, R. Bryant and D. O'Hallaron, All rights
 * reserved.  May not be used, modified, or copied without permission.
 *
 ********************************************************************/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "fcyc.h"
#include "defs.h"
#include "config.h"

/* Team structure that identifies the students */
extern team_t team; 
int seed;

/* Keep track of a number of different test functions */
#define MAX_BENCHMARKS 100
#define DIM_CNT 5

/* Misc constants */
#define BSIZE 32     /* cache block size in bytes */     
#define MAX_DIM 1280 /* 1024 + 256 */
#define ODD_DIM 96   /* not a power of 2 */

/* fast versions of min and max */
#define min(a,b) (a < b ? a : b)
#define max(a,b) (a > b ? a : b)

/* This struct characterizes the results for one benchmark test */
typedef struct {
    lab_test_func1 tfunct; /* The test function */
    double cpes[DIM_CNT]; /* One CPE result for each dimension */
    char *description;    /* ASCII description of the test function */
    unsigned short valid; /* The function is tested if this is non zero */
} bench_bokeh_t;

/* This struct characterizes the results for one benchmark test */
typedef struct {
    lab_test_func2 tfunct; /* The test function */
    double cpes[DIM_CNT]; /* One CPE result for each dimension */
    char *description;    /* ASCII description of the test function */
    unsigned short valid; /* The function is tested if this is non zero */
} bench_hadamard_t;

/* The range of image dimensions that we will be testing */
static int test_dim_bokeh[] = {64, 128, 256, 512, 1024};
static int test_dim_hadamard[] = {64, 128, 256, 512, 1024};

/* Baseline CPEs (see config.h) */
static double bokeh_baseline_cpes[] = {B32, B64, B128, B256, B512};
static double hadamard_baseline_cpes[] = {H64, H128, H256, H512, H1024};

/* These hold the results for all benchmarks */
static bench_bokeh_t benchmarks_bokeh[MAX_BENCHMARKS];
static bench_hadamard_t benchmarks_hadamard[MAX_BENCHMARKS];

/* These give the sizes of the above lists */
static int bokeh_benchmark_count = 0;
static int hadamard_benchmark_count = 0;

/* 
 * An image is a dimxdim matrix of pixels stored in a 1D array.  The
 * data array holds three images (the input original, a copy of the original, 
 * and the output result array. There is also an additional BSIZE bytes
 * of padding for alignment to cache block boundaries.
 */
static pixel image_data[(3*MAX_DIM*MAX_DIM) + (BSIZE/sizeof(pixel))];
static short filter_data[(2*MAX_DIM*MAX_DIM) + (BSIZE/sizeof(short))];
static int matrix_data[(5*MAX_DIM*MAX_DIM) + (BSIZE/sizeof(int))];

/* Various image pointers */
static pixel *orig_img = NULL;         		/* original image */
static pixel *copy_of_orig_img = NULL; 		/* copy of original image for checking result */
static short *orig_filter = NULL;	   		/* original filter */
static short *copy_of_orig_filter = NULL;	/* copy of original filter for checking result */
static pixel *result_img = NULL;       		/* result image */

/* Various matrix pointers */
static int *orig_m1 = NULL; 				/* original 1. matrix */
static int *orig_m2 = NULL; 				/* original 2. matrix */
static int *copy_of_orig_m1 = NULL;			/* copy original 1. matrix for checking result */
static int *copy_of_orig_m2 = NULL;			/* copy original 2. matrix for checking result */
static int *result_matrix;					/* result matrix */

/* Keep track of the best bokeh and hadamard score for grading */
double bokeh_maxmean = 0.0;
char *bokeh_maxmean_desc = NULL;

double hadamard_maxmean = 0.0;
char *hadamard_maxmean_desc = NULL;


/******************** Functions begin *************************/

void add_hadamard_function(lab_test_func2 f, char *description) 
{
    benchmarks_hadamard[hadamard_benchmark_count].tfunct = f;
    benchmarks_hadamard[hadamard_benchmark_count].description = description;
    benchmarks_hadamard[hadamard_benchmark_count].valid = 0;  
    hadamard_benchmark_count++;
}


void add_bokeh_function(lab_test_func1 f, char *description) 
{
    benchmarks_bokeh[bokeh_benchmark_count].tfunct = f;
    benchmarks_bokeh[bokeh_benchmark_count].description = description;
    benchmarks_bokeh[bokeh_benchmark_count].valid = 0;
    bokeh_benchmark_count++;
}

/* 
 * random_in_interval - Returns random integer in interval [low, high) 
 */
static int random_in_interval(int low, int high) 
{
    int size = high - low;
    return (rand()% size) + low;
}


/* 
 * random_in_interval - Returns sequence of integers in starting from <start> and with <step> size 
 */
static int* sequence_in_interval(int start, int step, int count) {
	int *res = (int *)calloc(count, sizeof(int));
	int i;
	int current = start;

	for ( i=0 ; i<count ; i++ ) {
		res[i] = current;
		current += step; 
	}
	return res;
}

/*
 * create a specific number of bokeh elements
 */

static void create_bokeh(int dim) {
	int row_count, block_count;
	int i, j, col;
	int *rows, *blocks;

	if ( dim == 96 )
		row_count = (dim/4);
	else
		row_count = 0;
	block_count = 2;
	rows = sequence_in_interval(0, 2, row_count);
	for ( i=0 ; i<row_count ; i++ ) {
		blocks = sequence_in_interval(0, 1, block_count);
		for ( j=0; j<block_count; j++ ) {
			col = blocks[j]*8;

			/* BOKEH BLOCK FOR ORIGINAL */
			orig_filter[RIDX(rows[i], col, 		dim)] 	= 0;
			orig_filter[RIDX(rows[i], col+1, 	dim)] 	= 0;
			orig_filter[RIDX(rows[i], col+2, 	dim)] 	= 0;
			orig_filter[RIDX(rows[i], col+3, 	dim)] 	= 0;
			orig_filter[RIDX(rows[i], col+4, 	dim)] 	= 0;
			orig_filter[RIDX(rows[i], col+5, 	dim)] 	= 0;
			orig_filter[RIDX(rows[i], col+6, 	dim)] 	= 0;
			orig_filter[RIDX(rows[i], col+7, 	dim)] 	= 0;

			/* BOKEH BLOCK FOR THE COPY */
			copy_of_orig_filter[RIDX(rows[i], col, 		dim)] 	= 0;
			copy_of_orig_filter[RIDX(rows[i], col+1, 	dim)] 	= 0;
			copy_of_orig_filter[RIDX(rows[i], col+2, 	dim)] 	= 0;
			copy_of_orig_filter[RIDX(rows[i], col+3, 	dim)] 	= 0;
			copy_of_orig_filter[RIDX(rows[i], col+4, 	dim)] 	= 0;
			copy_of_orig_filter[RIDX(rows[i], col+5, 	dim)] 	= 0;
			copy_of_orig_filter[RIDX(rows[i], col+6, 	dim)] 	= 0;
			copy_of_orig_filter[RIDX(rows[i], col+7, 	dim)] 	= 0;
		}
		free(blocks);
	}
	free(rows);
}

/*
 * create_image_and_filter - creates a dimxdim image and its filter aligned to a BSIZE byte boundary
 */
static void create_image_and_filter(int dim)
{
    int i, j;
    
    srand(seed);
    /* Align the images to BSIZE byte boundaries */
    orig_img = image_data;
    
    while ((unsigned)orig_img % BSIZE)
		orig_img++;
    result_img = orig_img + dim*dim;
    copy_of_orig_img = result_img + dim*dim;

    /* Align the filters to the BSIZE byte boundary */
    orig_filter = filter_data;
    while ((unsigned)orig_filter % BSIZE)
    	orig_filter++;
    copy_of_orig_filter = orig_filter + dim*dim;
    
    for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
		    /* Original image initialized to random colors */
		    orig_img[RIDX(i,j,dim)].red = random_in_interval(0, 65536);
		    orig_img[RIDX(i,j,dim)].green = random_in_interval(0, 65536);
		    orig_img[RIDX(i,j,dim)].blue = random_in_interval(0, 65536);

		    /* Copy of original image for checking result */
		    copy_of_orig_img[RIDX(i,j,dim)].red = orig_img[RIDX(i,j,dim)].red;
		    copy_of_orig_img[RIDX(i,j,dim)].green = orig_img[RIDX(i,j,dim)].green;
		    copy_of_orig_img[RIDX(i,j,dim)].blue = orig_img[RIDX(i,j,dim)].blue;

		    /* Result image initialized to all black */
		    result_img[RIDX(i,j,dim)].red = 0;
		    result_img[RIDX(i,j,dim)].green = 0;
		    result_img[RIDX(i,j,dim)].blue = 0;

		    /* Filter is created with fully focused at the beginning */
		    orig_filter[RIDX(i,j,dim)] = 0;
		    copy_of_orig_filter[RIDX(i,j,dim)] = 0;
		    
		}
    }

    create_bokeh(dim);

    return;
}

/*
 * create_matrix - creates two dimxdim matrices aligned to a BSIZE byte boundary
 */
static void create_matrix(int dim)
{
    int i, j;
    srand(seed);
    /* Align the images to BSIZE byte boundaries */
    orig_m1 = matrix_data;
    while ((unsigned)orig_m1 % BSIZE)
		orig_m1++;
    orig_m2 = orig_m1 + dim*dim;
    result_matrix = orig_m2 + dim*dim;
    copy_of_orig_m1 = result_matrix + dim*dim;
    copy_of_orig_m2 = copy_of_orig_m1 + dim*dim;

    for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
		    /* Original matrices initialized to random integers */
		    orig_m1[RIDX(i,j,dim)]	 		= random_in_interval(-128, 128);
		    orig_m2[RIDX(i,j,dim)] 			= random_in_interval(-128, 128);

		    /* Copy of original matrices for checking result */
		    copy_of_orig_m1[RIDX(i,j,dim)] 	= orig_m1[RIDX(i,j,dim)];
		    copy_of_orig_m2[RIDX(i,j,dim)]	= orig_m2[RIDX(i,j,dim)];
		   
		    /* Result image initialized to all 0 */
		    result_matrix[RIDX(i,j,dim)] = 0;
		}
    }

    return;
}


/* 
 * compare_pixels - Returns 1 if the two arguments don't have same RGB
 *    values, 0 o.w.  
 */
static int compare_pixels(pixel p1, pixel p2) 
{
    return 
	(p1.red != p2.red) || 
	(p1.green != p2.green) || 
	(p1.blue != p2.blue);
}


/* Make sure the orig array is unchanged */
static int check_orig_img_and_filter(int dim) 
{
    int i, j;

    for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) { 
		    if (compare_pixels(orig_img[RIDX(i,j,dim)], copy_of_orig_img[RIDX(i,j,dim)])) {
				printf("\n");
				printf("Error: Original image has been changed!\n");
				return 1;
		    }
		}
	}

    return 0;
}

/* Make sure the orig array is unchanged */
static int check_orig_matrix(int dim) 
{
    int i, j;

    for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
		    if ( orig_m1[RIDX(i,j,dim)] != copy_of_orig_m1[RIDX(i,j,dim)] ) {
				printf("\n");
				printf("Error: Original matrix 1 has been changed!\n");
				return 1;
		    }
		    if ( orig_m2[RIDX(i,j,dim)] != copy_of_orig_m2[RIDX(i,j,dim)] ) {
				printf("\n");
				printf("Error: Original matrix 2 has been changed!\n");
				return 1;
		    }
		}
    }

    return 0;
}
static pixel check_average(int dim, int i, int j, pixel *src) {
    pixel result;
    int num = 0;
    int ii, jj;
    int sum0, sum1, sum2;
    int top_left_i, top_left_j;
    int bottom_right_i, bottom_right_j;

    top_left_i = max(i-1, 0);
    top_left_j = max(j-1, 0);
    bottom_right_i = min(i+1, dim-1); 
    bottom_right_j = min(j+1, dim-1);

    sum0 = sum1 = sum2 = 0;
    for(ii=top_left_i; ii <= bottom_right_i; ii++) {
	for(jj=top_left_j; jj <= bottom_right_j; jj++) {
	    num++;
	    sum0 += (int) src[RIDX(ii,jj,dim)].red;
	    sum1 += (int) src[RIDX(ii,jj,dim)].green;
	    sum2 += (int) src[RIDX(ii,jj,dim)].blue;
	}
    }
    result.red = (unsigned short) (sum0/num);
    result.green = (unsigned short) (sum1/num);
    result.blue = (unsigned short) (sum2/num);
 
    return result;
}


/* 
 * check_bokeh - Make sure the bokeh function actually works.  The
 * orig array should not have been tampered with!  
 */
static int check_bokeh(int dim) {
    int err = 0;
    int i, j;
    int badi = 0;
    int badj = 0;
    pixel right, wrong;

    /* return 1 if original image has been changed */
    if (check_orig_img_and_filter(dim)) 
		return 1; 

    for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			pixel bokehed;
			if ( orig_filter[(RIDX(i, j, dim))] )
				bokehed = orig_img[RIDX(i,j,dim)];
			else
		    	bokehed = check_average(dim, i, j, orig_img);
		    if (compare_pixels(result_img[RIDX(i,j,dim)], bokehed)) {
				err++;
				badi = i;
				badj = j;
				wrong = result_img[RIDX(i,j,dim)];
				right = bokehed;
				break;
		    }
		}
		if (err) break;
    }

    if (err) {
		printf("\n");
		printf("ERROR: Dimension=%d, %d errors\n", dim, err);    
		printf("E.g., \n");
		printf("You have dst[%d][%d].{red,green,blue} = {%d,%d,%d}\n",
		       badi, badj, wrong.red, wrong.green, wrong.blue);
		printf("It should be dst[%d][%d].{red,green,blue} = {%d,%d,%d}\n",
		       badi, badj, right.red, right.green, right.blue);
    }

    return err;
}

/* 
 * check_hadamard - Make sure the hadamard actually works. 
 * The orig array should not  have been tampered with! 
 */
static int check_hadamard(int dim) 
{
    int err = 0;
    int i, j;
    int badi = 0;
    int badj = 0;
    int m1_bad, m2_bad, res_bad;
    int correct_mult;

    /* return 1 if the original image has been  changed */
    if (check_orig_matrix(dim)) 
		return 1; 

    for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			correct_mult = orig_m1[RIDX(i,j,dim)] * orig_m2[RIDX(i,j,dim)];
		    if ( correct_mult != result_matrix[RIDX(i,j,dim)] ) {
				err++;
				badi = i;
				badj = j;
				m1_bad = orig_m1[RIDX(i,j,dim)];
				m2_bad = orig_m2[RIDX(i,j,dim)];
				res_bad = result_matrix[RIDX(i,j,dim)];
				break;
		    }
		}
		if (err) break;
    }

    if (err) {
		printf("\n");
		printf("ERROR: Dimension=%d, %d errors\n", dim, err);    
		printf("E.g., The following two values multipled to the wrong result:\n");
		printf("src1[%d][%d] = %d | src2[%d][%d] = %d\n",
		       badi, badj, m1_bad, badi, badj, m2_bad);
		printf("You have dst[%d][%d] = %d and it should be dst[%d][%d] = %d\n",
		       badi, badj, res_bad, badi, badj, correct_mult);
    }

    return err;
}




void func_wrapper_bokeh(void *arglist[]) 
{
    pixel *src, *dst;
    short *filter;
    int mydim;
    lab_test_func1 f;

    f = (lab_test_func1) arglist[0];
    mydim = *((int *) arglist[1]);
    src = (pixel *) arglist[2];
    filter = (short *) arglist[3];
    dst = (pixel *) arglist[4];

    (*f)(mydim, src, filter, dst);

    return;
}

void func_wrapper_hadamard(void *arglist[]) 
{
    int *src1, *src2, *dst;
    int mydim;
    lab_test_func2 f;

    f = (lab_test_func2) arglist[0];
    mydim = *((int *) arglist[1]);
    src1 = (int *) arglist[2];
    src2 = (int *) arglist[3];
    dst = (int *) arglist[4];

    (*f)(mydim, src1, src2, dst);

    return;
}

void run_bokeh_benchmark(int idx, int dim) 
{
    benchmarks_bokeh[idx].tfunct(dim, orig_img, orig_filter, result_img);
}

void test_bokeh(int bench_index) 
{
    int i;
    int test_num;
    char *description = benchmarks_bokeh[bench_index].description;
  
    for(test_num=0; test_num < DIM_CNT; test_num++) {
	int dim;

	/* Check correctness for odd (non power of two dimensions */
	create_image_and_filter(ODD_DIM);
	run_bokeh_benchmark(bench_index, ODD_DIM);
	if (check_bokeh(ODD_DIM)) {
	    printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
		   benchmarks_bokeh[bench_index].description, ODD_DIM);
	    return;
	}

	/* Create a test image of the required dimension */
	dim = test_dim_bokeh[test_num];
	create_image_and_filter(dim);

#ifdef DEBUG
	printf("DEBUG: Running benchmark \"%s\"\n", benchmarks_bokeh[bench_index].description);
#endif
	/* Check that the code works */
	run_bokeh_benchmark(bench_index, dim);
	if (check_bokeh(dim)) {
	    printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
		   benchmarks_bokeh[bench_index].description, dim);
	    return;
	}

	/* Measure CPE */
	{
	    double num_cycles, cpe;
	    int tmpdim = dim;
	    void *arglist[5];
	    double dimension = (double) dim;
	    double work = dimension*dimension;
#ifdef DEBUG
	    printf("DEBUG: dimension=%.1f\n",dimension);
	    printf("DEBUG: work=%.1f\n",work);
#endif
	    arglist[0] = (void *) benchmarks_bokeh[bench_index].tfunct;
	    arglist[1] = (void *) &tmpdim;
	    arglist[2] = (void *) orig_img;
	    arglist[3] = (void *) orig_filter;
	    arglist[4] = (void *) result_img;
        
	    create_image_and_filter(dim);
	    num_cycles = fcyc_v((test_funct_v)&func_wrapper_bokeh, arglist); 
	    cpe = num_cycles/work;
	    benchmarks_bokeh[bench_index].cpes[test_num] = cpe;
	}
    }

    /* Print results as a table */
    printf("Bokeh: Version = %s:\n", description);
    printf("Dim\t");
    for (i = 0; i < DIM_CNT; i++)
	printf("\t%d", test_dim_bokeh[i]);
    printf("\tMean\n");
  
    printf("Your CPEs");
    for (i = 0; i < DIM_CNT; i++) {
	printf("\t%.1f", benchmarks_bokeh[bench_index].cpes[i]);
    }
    printf("\n");

    printf("Baseline CPEs");
    for (i = 0; i < DIM_CNT; i++) {
	printf("\t%.1f", bokeh_baseline_cpes[i]);
    }
    printf("\n");

    /* Compute speedup */
    {
	double prod, ratio, mean;
	prod = 1.0; /* Geometric mean */
	printf("Speedup\t");
	for (i = 0; i < DIM_CNT; i++) {
	    if (benchmarks_bokeh[bench_index].cpes[i] > 0.0) {
		ratio = bokeh_baseline_cpes[i]/
		    benchmarks_bokeh[bench_index].cpes[i];
	    }
	    else {
		printf("Fatal Error: Non-positive CPE value...\n");
		exit(EXIT_FAILURE);
	    }
	    prod *= ratio;
	    printf("\t%.1f", ratio);
	}
	/* Geometric mean */
	mean = pow(prod, 1.0/(double) DIM_CNT);
	printf("\t%.1f", mean);
	printf("\n\n");
	if (mean > bokeh_maxmean) {
	    bokeh_maxmean = mean;
	    bokeh_maxmean_desc = benchmarks_bokeh[bench_index].description;
	}
    }

    return;  
}

void run_hadamard_benchmark(int idx, int dim) 
{
    benchmarks_hadamard[idx].tfunct(dim, orig_m1, orig_m2, result_matrix);
}

void test_hadamard(int bench_index) 
{
    int i;
    int test_num;
    char *description = benchmarks_hadamard[bench_index].description;
  
    for (test_num = 0; test_num < DIM_CNT; test_num++) {
	int dim;

	/* Check for odd dimension */
	create_matrix(ODD_DIM);
	run_hadamard_benchmark(bench_index, ODD_DIM);
	if (check_hadamard(ODD_DIM)) {
	    printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
		   benchmarks_hadamard[bench_index].description, ODD_DIM);
	    return;
	}

	/* Create a test image of the required dimension */
	dim = test_dim_hadamard[test_num];
	create_matrix(dim);
#ifdef DEBUG
	printf("DEBUG: Running benchmark \"%s\"\n", benchmarks_hadamard[bench_index].description);
#endif

	/* Check that the code works */
	run_hadamard_benchmark(bench_index, dim);
	if (check_hadamard(dim)) {
	    printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
		   benchmarks_hadamard[bench_index].description, dim);
	    return;
	}

	/* Measure CPE */
	{
	    double num_cycles, cpe;
	    int tmpdim = dim;
	    void *arglist[5];
	    double dimension = (double) dim;
	    double work = dimension*dimension;
#ifdef DEBUG
	    printf("DEBUG: dimension=%.1f\n",dimension);
	    printf("DEBUG: work=%.1f\n",work);
#endif
	    arglist[0] = (void *) benchmarks_hadamard[bench_index].tfunct;
	    arglist[1] = (void *) &tmpdim;
	    arglist[2] = (void *) orig_m1;
	    arglist[3] = (void *) orig_m2;
	    arglist[4] = (void *) result_matrix;

	    create_matrix(dim);
	    num_cycles = fcyc_v((test_funct_v)&func_wrapper_hadamard, arglist); 
	    cpe = num_cycles/work;
	    benchmarks_hadamard[bench_index].cpes[test_num] = cpe;
	}
    }

    /* 
     * Print results as a table 
     */
    printf("Hadamard: Version = %s:\n", description);
    printf("Dim\t");
    for (i = 0; i < DIM_CNT; i++)
	printf("\t%d", test_dim_hadamard[i]);
    printf("\tMean\n");
  
    printf("Your CPEs");
    for (i = 0; i < DIM_CNT; i++) {
	printf("\t%.1f", benchmarks_hadamard[bench_index].cpes[i]);
    }
    printf("\n");

    printf("Baseline CPEs");
    for (i = 0; i < DIM_CNT; i++) {
	printf("\t%.1f", hadamard_baseline_cpes[i]);
    }
    printf("\n");

    /* Compute Speedup */
    {
	double prod, ratio, mean;
	prod = 1.0; /* Geometric mean */
	printf("Speedup\t");
	for (i = 0; i < DIM_CNT; i++) {
	    if (benchmarks_hadamard[bench_index].cpes[i] > 0.0) {
		ratio = hadamard_baseline_cpes[i]/
		    benchmarks_hadamard[bench_index].cpes[i];
	    }
	    else {
		printf("Fatal Error: Non-positive CPE value...\n");
		exit(EXIT_FAILURE);
	    }
	    prod *= ratio;
	    printf("\t%.1f", ratio);
	}

	/* Geometric mean */
	mean = pow(prod, 1.0/(double) DIM_CNT);
	printf("\t%.1f", mean);
	printf("\n\n");
	if (mean > hadamard_maxmean) {
	    hadamard_maxmean = mean;
	    hadamard_maxmean_desc = benchmarks_hadamard[bench_index].description;
	}
    }


#ifdef DEBUG
    fflush(stdout);
#endif
    return;  
}




void usage(char *progname) 
{
    fprintf(stderr, "Usage: %s [-hqg] [-f <func_file>] [-d <dump_file>]\n", progname);    
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -h         Print this message\n");
    fprintf(stderr, "  -q         Quit after dumping (use with -d )\n");
    fprintf(stderr, "  -g         Autograder mode: checks only bokeh() and hadamard()\n");
    fprintf(stderr, "  -f <file>  Get test function names from dump file <file>\n");
    fprintf(stderr, "  -d <file>  Emit a dump file <file> for later use with -f\n");
    exit(EXIT_FAILURE);
}



int main(int argc, char *argv[])
{
    int i;
    int quit_after_dump = 0;
    int skip_teamname_check = 0;
    int autograder = 0;
    seed = 1729;
    char c = '0';
    char *bench_func_file = NULL;
    char *func_dump_file = NULL;

    /* register all the defined functions */
    register_bokeh_functions();
    register_hadamard_functions();

    /* parse command line args */
    while ((c = getopt(argc, argv, "tgqf:d:s:h")) != -1)
	switch (c) {

	case 't': /* skip team name check (hidden flag) */
	    skip_teamname_check = 1;
	    break;

	case 's': /* seed for random number generator (hidden flag) */
	    seed = atoi(optarg);
	    break;

	case 'g': /* autograder mode (checks only bokeh() and hadamard()) */
	    autograder = 1;
	    break;

	case 'q':
	    quit_after_dump = 1;
	    break;

	case 'f': /* get names of benchmark functions from this file */
	    bench_func_file = strdup(optarg);
	    break;

	case 'd': /* dump names of benchmark functions to this file */
	    func_dump_file = strdup(optarg);
	    {
		int i;
		FILE *fp = fopen(func_dump_file, "w");	

		if (fp == NULL) {
		    printf("Can't open file %s\n",func_dump_file);
		    exit(-5);
		}

		for(i = 0; i < bokeh_benchmark_count; i++) {
		    fprintf(fp, "R:%s\n", benchmarks_bokeh[i].description); 
		}
		for(i = 0; i < hadamard_benchmark_count; i++) {
		    fprintf(fp, "S:%s\n", benchmarks_hadamard[i].description); 
		}
		fclose(fp);
	    }
	    break;

	case 'h': /* print help message */
	    usage(argv[0]);

	default: /* unrecognized argument */
	    usage(argv[0]);
	}

    if (quit_after_dump) 
	exit(EXIT_SUCCESS);


    /* Print team info */
    if (!skip_teamname_check) {
	if (strcmp("bovik", team.team) == 0) {
	    printf("%s: Please fill in the team struct in kernels.c.\n", argv[0]);
	    exit(1);
	}
	printf("Teamname: %s\n", team.team);
	printf("Member 1: %s\n", team.name1);
	printf("ID 1: %s\n", team.id1);
	if (*team.name2 || *team.id2) {
	    printf("Member 2: %s\n", team.name2);
	    printf("ID 2: %s\n", team.id2);
	}
	if (*team.name3 || *team.id3) {
	    printf("Member 3: %s\n", team.name3);
	    printf("ID 3: %s\n", team.id3);
	}
	printf("\n");
    }

    //srand(seed);

    /* 
     * If we are running in autograder mode, we will only test
     * the bokeh() and bench() functions.
     */
    if (autograder) {
	bokeh_benchmark_count = 1;
	hadamard_benchmark_count = 1;

	benchmarks_bokeh[0].tfunct = bokeh;
	benchmarks_bokeh[0].description = "bokeh() function";
	benchmarks_bokeh[0].valid = 1;

	benchmarks_hadamard[0].tfunct = hadamard;
	benchmarks_hadamard[0].description = "hadamard() function";
	benchmarks_hadamard[0].valid = 1;
    }

    /* 
     * If the user specified a file name using -f, then use
     * the file to determine the versions of bokeh and hadamard to test
     */
    else if (bench_func_file != NULL) {
	char flag;
	char func_line[256];
	FILE *fp = fopen(bench_func_file, "r");

	if (fp == NULL) {
	    printf("Can't open file %s\n",bench_func_file);
	    exit(-5);
	}
    
	while(func_line == fgets(func_line, 256, fp)) {
	    char *func_name = func_line;
	    char **strptr = &func_name;
	    char *token = strsep(strptr, ":");
	    flag = token[0];
	    func_name = strsep(strptr, "\n");
#ifdef DEBUG
	    printf("Function Description is %s\n",func_name);
#endif

	    if (flag == 'R') {
		for(i=0; i<bokeh_benchmark_count; i++) {
		    if (strcmp(benchmarks_bokeh[i].description, func_name) == 0)
			benchmarks_bokeh[i].valid = 1;
		}
	    }
	    else if (flag == 'S') {
		for(i=0; i<hadamard_benchmark_count; i++) {
		    if (strcmp(benchmarks_hadamard[i].description, func_name) == 0)
			benchmarks_hadamard[i].valid = 1;
		}
	    }      
	}

	fclose(fp);
    }

    /* 
     * If the user didn't specify a dump file using -f, then 
     * test all of the functions
     */
    else { /* set all valid flags to 1 */
	for (i = 0; i < bokeh_benchmark_count; i++)
	    benchmarks_bokeh[i].valid = 1;
	for (i = 0; i < hadamard_benchmark_count; i++)
	    benchmarks_hadamard[i].valid = 1;
    }

    /* Set measurement (fcyc) parameters */
    set_fcyc_cache_size(1 << 14); /* 16 KB cache size */
    set_fcyc_clear_cache(1); /* clear the cache before each measurement */
    set_fcyc_compensate(1); /* try to compensate for timer overhead */
 
    for (i = 0; i < bokeh_benchmark_count; i++) {
	if (benchmarks_bokeh[i].valid)
	    test_bokeh(i);
    
}
    for (i = 0; i < hadamard_benchmark_count; i++) {
	if (benchmarks_hadamard[i].valid)
	    test_hadamard(i);
    }


    if (autograder) {
	printf("\nbestscores:%.1f:%.1f:\n", bokeh_maxmean, hadamard_maxmean);
    }
    else {
	printf("Summary of Your Best Scores:\n");
	printf("  Bokeh: %3.1f (%s)\n", bokeh_maxmean, bokeh_maxmean_desc);
	printf("  Hadamard: %3.1f (%s)\n", hadamard_maxmean, hadamard_maxmean_desc);
    }

    return 0;
}













