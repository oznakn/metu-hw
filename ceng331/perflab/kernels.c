/********************************************************
 * Kernels to be optimized for the Metu Ceng Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/*
 * Please fill in the following team struct
 */
team_t team = {
    "Team",                     /* Team name */

    "Alp Eren Yılmaz",             /* First member full name */
    "2310662",                 /* First member id */

    "Ozan Akın",                         /* Second member full name (leave blank if none) */
    "e2309599",                         /* Second member id (leave blank if none) */

    "Yiğitcan Uçan",                         /* Third member full name (leave blank if none) */
    "e2310555"                          /* Third member id (leave blank if none) */
};

/****************
 * BOKEH KERNEL *
 ****************/

/****************************************************************
 * Various typedefs and helper functions for the bokeh function
 * You may modify these any way you like.
 ****************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* pixel_sum without num */
typedef struct {
    int red;
    int green;
    int blue;
} pixel_sum_yiit;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p)
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++)
    for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++)
        accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/*******************************************************
 * Your different versions of the bokeh kernel go here
 *******************************************************/

/*
 * naive_bokeh - The naive baseline version of bokeh effect with filter
 */
char naive_bokeh_descr[] = "naive_bokeh: Naive baseline bokeh with filter";
void naive_bokeh(int dim, pixel *src, short *flt, pixel *dst) {

    int i, j;

    for(i = 0; i < dim; i++) {
        for(j = 0; j < dim; j++) {
            if ( !flt[RIDX(i, j, dim)] )
                dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
            else
                dst[RIDX(i, j, dim)] = src[RIDX(i, j, dim)];
        }
    }
}

/*
 * bokeh - Your current working version of bokeh
 * IMPORTANT: This is the version you will be graded on
 */
char bokeh_descr[] = "bokeh: Current working version";
void bokeh(int dim, pixel *src, short *flt, pixel *dst)
{
    naive_bokeh(dim, src, flt, dst);
}

pixel_sum_yiit sum3[1024][1024];
char ucanyiit_bokeh_descr[] = "ucanyiit_bokeh";
void ucanyiit_bokeh(int dim, pixel *src, short *flt, pixel *dst)
{
    int i = 0, j, dim_minus_1 = dim-1, n = 0, x;

    // preproccess
    for(;i<dim;i++){
        pixel_sum_yiit sum;
        pixel src1 = src[n++], src2 = src[n++], src3=src[n++];

        sum.red = src1.red + src2.red;
        sum.green = src1.green + src2.green;
        sum.blue = src1.blue + src2.blue;
        sum3[i][0] = sum;

        sum.red += src3.red;
        sum.green += src3.green;
        sum.blue += src3.blue;
        sum3[i][1] = sum;

        for(j=2;j<dim_minus_1;j++, n++){
            pixel rem = src[n-3], add = src[n];
            sum.red -= rem.red;
            sum.green -= rem.green;
            sum.blue -= rem.blue;

            sum.red += add.red;
            sum.green += add.green;
            sum.blue += add.blue;

            sum3[i][j] = sum;
        }
        pixel rem = src[n-3];
        sum.red -= rem.red;
        sum.green -= rem.green;
        sum.blue -= rem.blue;
        sum3[i][dim_minus_1] = sum;
    }

    // first row - first column
    n=0; i=0;
    if(flt[n]) dst[n] = src[n];
    else {
        pixel_sum_yiit sum = sum3[0][0], next = sum3[1][0];
        x = 4;

        sum.red += next.red;
        sum.green += next.green;
        sum.blue += next.blue;

        dst[n].red = (unsigned short)(sum.red/x);
        dst[n].green = (unsigned short)(sum.green/x);
        dst[n].blue = (unsigned short)(sum.blue/x);
    }

    // first row - 2nd to (n-1)th column
    n++; x = 6;
    for(j = 1; j < dim_minus_1; j++, n++) {
        if(flt[n]) dst[n] = src[n];
        else {
            pixel_sum_yiit sum = sum3[0][j], next = sum3[1][j];;

            sum.red += next.red;
            sum.green += next.green;
            sum.blue += next.blue;

            dst[n].red = (unsigned short)(sum.red/x);
            dst[n].green = (unsigned short)(sum.green/x);
            dst[n].blue = (unsigned short)(sum.blue/x);
        }
    }

    // first row - last column
    if(flt[n]) dst[n] = src[n];
    else {
        pixel_sum_yiit sum = sum3[0][j], next = sum3[1][j];
        x = 4;

        sum.red += next.red;
        sum.green += next.green;
        sum.blue += next.blue;

        dst[n].red = (unsigned short)(sum.red/x);
        dst[n].green = (unsigned short)(sum.green/x);
        dst[n].blue = (unsigned short)(sum.blue/x);
    }
    n++;

    // 2nd to (n-1)th row
    for(i = 1; i < dim_minus_1; i++) {

        // 2nd to (n-1)th row - first column
        if(flt[n]) dst[n] = src[n];
        else {
            pixel_sum_yiit prev = sum3[i-1][0], sum = sum3[i][0], next = sum3[i+1][0];
            x = 6;

            sum.red += prev.red;
            sum.green += prev.green;
            sum.blue += prev.blue;

            sum.red += next.red;
            sum.green += next.green;
            sum.blue += next.blue;

            dst[n].red = (unsigned short)(sum.red/x);
            dst[n].green = (unsigned short)(sum.green/x);
            dst[n].blue = (unsigned short)(sum.blue/x);
        }

        // 2nd to (n-1)th row - 2nd to (n-1)th column
        n++; x = 9;
        for(j=1; j < dim_minus_1; j++, n++) {
            if(flt[n]) dst[n] = src[n];
            else {
                pixel_sum_yiit prev = sum3[i-1][j], sum = sum3[i][j], next = sum3[i+1][j];

                sum.red += prev.red;
                sum.green += prev.green;
                sum.blue += prev.blue;

                sum.red += next.red;
                sum.green += next.green;
                sum.blue += next.blue;

                dst[n].red = (unsigned short)(sum.red/x);
                dst[n].green = (unsigned short)(sum.green/x);
                dst[n].blue = (unsigned short)(sum.blue/x);
            }
        }

        // 2nd to (n-1)th row - last column
        if(flt[n]) dst[n] = src[n];
        else {
            pixel_sum_yiit prev = sum3[i-1][j], sum = sum3[i][j], next = sum3[i+1][j];
            int x = 6;

            sum.red += prev.red;
            sum.green += prev.green;
            sum.blue += prev.blue;

            sum.red += next.red;
            sum.green += next.green;
            sum.blue += next.blue;

            dst[n].red = (unsigned short)(sum.red/x);
            dst[n].green = (unsigned short)(sum.green/x);
            dst[n].blue = (unsigned short)(sum.blue/x);
        }
        n++;
    }

    // last row - first column
    if(flt[n]) dst[n] = src[n];
    else {
        pixel_sum_yiit prev = sum3[i-1][0], sum = sum3[i][0];
        int x = 4;

        sum.red += prev.red;
        sum.green += prev.green;
        sum.blue += prev.blue;

        dst[n].red = (unsigned short)(sum.red/x);
        dst[n].green = (unsigned short)(sum.green/x);
        dst[n].blue = (unsigned short)(sum.blue/x);
    }

    // last row - 2nd to (n-1)th column
    n++; x = 6;
    for(j = 1; j < dim_minus_1; j++, n++) {
        if(flt[n]) dst[n] = src[n];
        else {
            pixel_sum_yiit prev = sum3[i-1][j], sum = sum3[i][j];

            sum.red += prev.red;
            sum.green += prev.green;
            sum.blue += prev.blue;

            dst[n].red = (unsigned short)(sum.red/x);
            dst[n].green = (unsigned short)(sum.green/x);
            dst[n].blue = (unsigned short)(sum.blue/x);
        }
    }

    // last row - last column
    if(flt[n]) dst[n] = src[n];
    else {
        pixel_sum_yiit prev = sum3[i-1][j], sum = sum3[i][j];
        int x = 4;

        sum.red += prev.red;
        sum.green += prev.green;
        sum.blue += prev.blue;

        dst[n].red = (unsigned short)(sum.red/x);
        dst[n].green = (unsigned short)(sum.green/x);
        dst[n].blue = (unsigned short)(sum.blue/x);
    }

}

//
//
// Alp's region starts
//
//

// Big change comes!
char alp_bokeh_descr[] = "alp_bokeh";
void alp_bokeh(int dim, pixel *src, short *flt, pixel *dst)
{
    int i, j;
    int n = dim+1;
    int dim1 = dim - 1;
    int blue_sum,green_sum,red_sum;

    for(i = 1; i < dim1; i++) {
        for(j = 1; j < dim1; j++) {
            if ( !flt[n] ){
                //dst[n]= new_avg(dim, i, j, src);
                // Ugly loop unroll
                blue_sum = 0;
                green_sum = 0;
                red_sum = 0;

                // I think gcc will optimize this more
                int index = (i-1)*dim + j-1;
                // First pixel
                blue_sum += src[index].blue;
                green_sum += src[index].green;
                red_sum += src[index].red;
                // Second pixel
                blue_sum += src[index+1].blue;
                green_sum += src[index+1].green;
                red_sum += src[index+1].red;
                // Third pixel
                blue_sum += src[index+2].blue;
                green_sum += src[index+2].green;
                red_sum += src[index+2].red;

                index += dim;
                // First pixel
                blue_sum += src[index].blue;
                green_sum += src[index].green;
                red_sum += src[index].red;
                // Second pixel
                blue_sum += src[index+1].blue;
                green_sum += src[index+1].green;
                red_sum += src[index+1].red;
                // Third pixel
                blue_sum += src[index+2].blue;
                green_sum += src[index+2].green;
                red_sum += src[index+2].red;

                index += dim;
                // First pixel
                blue_sum += src[index].blue;
                green_sum += src[index].green;
                red_sum += src[index].red;
                // Second pixel
                blue_sum += src[index+1].blue;
                green_sum += src[index+1].green;
                red_sum += src[index+1].red;
                // Third pixel
                blue_sum += src[index+2].blue;
                green_sum += src[index+2].green;
                red_sum += src[index+2].red;

                blue_sum /= 9;
                green_sum /= 9;
                red_sum /= 9;

                dst[n].blue = blue_sum;
                dst[n].red = red_sum;
                dst[n].green = green_sum;
            }
            else
                dst[n] = src[n];
            n++;
        }
        n+=2;
    }

    // Case'leri ayır
    // Writing this way to help compiler more...


    // i = 0 j =0
    if(!flt[0]){
        blue_sum  = src[0].blue;
        green_sum = src[0].green;
        red_sum   = src[0].red;

        blue_sum  += src[1].blue;
        green_sum += src[1].green;
        red_sum   += src[1].red;

        blue_sum  += src[dim].blue;
        green_sum += src[dim].green;
        red_sum   += src[dim].red;

        blue_sum  += src[dim+1].blue;
        green_sum += src[dim+1].green;
        red_sum   += src[dim+1].red;

        dst[0].blue  = blue_sum/4;
        dst[0].green = green_sum/4;
        dst[0].red   = red_sum/4;
        //printf("%d, %d, %d\n", src[0].blue, src[0].green, src[0].red);
        //printf("%d, %d, %d\n", src[1].blue, src[1].green, src[1].red);
        //printf("%d, %d, %d\n", src[dim].blue, src[dim].green, src[dim].red);
        //printf("%d, %d, %d\n", src[dim+1].blue, src[dim+1].green, src[dim+1].red);
        //printf("%d, %d, %d\n\n\n", dst[0].blue, dst[0].green, dst[0].red);
    }
    else{
        dst[0] = src[0];
    }

    // i = 0 j = dim-1
    if(!flt[dim-1]){
        blue_sum  = src[dim-2].blue;
        green_sum = src[dim-2].green;
        red_sum   = src[dim-2].red;

        blue_sum  += src[dim-1].blue;
        green_sum += src[dim-1].green;
        red_sum   += src[dim-1].red;

        blue_sum  += src[dim*2-2].blue; // I think gcc will optimize this, but may need to check
        green_sum += src[dim*2-2].green;
        red_sum   += src[dim*2-2].red;

        blue_sum  += src[dim*2-1].blue;
        green_sum += src[dim*2-1].green;
        red_sum   += src[dim*2-1].red;

        dst[dim-1].blue  = blue_sum/4;
        dst[dim-1].green = green_sum/4;
        dst[dim-1].red   = red_sum/4;
        //printf("%d, %d, %d\n", dst[dim-1].blue, dst[dim-1].green, dst[dim-1].red);
    }
    else{
        dst[dim-1] = src[dim-1];
    }
    // i = dim-1 j = 0
    int temp1 = dim*(dim-1);
    if(!flt[temp1]){
        blue_sum  = src[temp1].blue;
        green_sum = src[temp1].green;
        red_sum   = src[temp1].red;

        blue_sum  += src[temp1-dim].blue; // Again, these should optimized
        green_sum += src[temp1-dim].green;
        red_sum   += src[temp1-dim].red;

        blue_sum  += src[temp1-dim+1].blue;
        green_sum += src[temp1-dim+1].green;
        red_sum   += src[temp1-dim+1].red;

        blue_sum  += src[temp1+1].blue;
        green_sum += src[temp1+1].green;
        red_sum   += src[temp1+1].red;

        dst[temp1].blue  = blue_sum/4;
        dst[temp1].green = green_sum/4;
        dst[temp1].red   = red_sum/4;
        //printf("%d, %d, %d\n", dst[temp1].blue, dst[temp1].green, dst[temp1].red);
    }
    else{
        dst[temp1] = src[temp1];
    }

    temp1 += dim-1;
    // i = dim-1 j = dim-1
    if(!flt[temp1]){
        blue_sum  = src[temp1].blue;
        green_sum = src[temp1].green;
        red_sum   = src[temp1].red;

        blue_sum  += src[temp1-dim].blue; // Again, these should optimized
        green_sum += src[temp1-dim].green;
        red_sum   += src[temp1-dim].red;

        blue_sum  += src[temp1-dim-1].blue;
        green_sum += src[temp1-dim-1].green;
        red_sum   += src[temp1-dim-1].red;

        blue_sum  += src[temp1-1].blue;
        green_sum += src[temp1-1].green;
        red_sum   += src[temp1-1].red;

        dst[temp1].blue  = blue_sum/4;
        dst[temp1].green = green_sum/4;
        dst[temp1].red   = red_sum/4;
        //printf("%d, %d, %d\n", dst[temp1].blue, dst[temp1].green, dst[temp1].red);
    }
    else{
        dst[temp1] = src[temp1];
    }
    // Now, others

    // i=0 case
    i = 0;
    for(j = 1; j < (dim-1); j++){
        //dst[j]= new_avg(dim, i, j, src);
        if(flt[j]){
            dst[j] = src[j];
            continue;
        }
        // our position is src[j]
        // Please gcc, optimize, do whatever you can...
        blue_sum  = src[j-1].blue;
        green_sum = src[j-1].green;
        red_sum   = src[j-1].red;

        blue_sum  += src[j].blue;
        green_sum += src[j].green;
        red_sum   += src[j].red;

        blue_sum  += src[j+1].blue;
        green_sum += src[j+1].green;
        red_sum   += src[j+1].red;

        int temp = dim + j;
        blue_sum  += src[temp-1].blue;
        green_sum += src[temp-1].green;
        red_sum   += src[temp-1].red;

        blue_sum  += src[temp].blue;
        green_sum += src[temp].green;
        red_sum   += src[temp].red;

        blue_sum  += src[temp+1].blue;
        green_sum += src[temp+1].green;
        red_sum   += src[temp+1].red;

        dst[j].blue  = blue_sum/6;
        dst[j].green = green_sum/6;
        dst[j].red   = red_sum/6;

    }

    // j = 0 case
    j = 0;
    n = dim;
    for(i = 1; i < (dim-1); i++,n+=dim){
        //dst[n] = new_avg(dim,i,j,src);
        //n+=dim;
        if(flt[n]){
            dst[n] = src[n];
            continue;
        }
        blue_sum  = src[n-dim].blue;
        green_sum = src[n-dim].green;
        red_sum   = src[n-dim].red;

        blue_sum  += src[n-dim+1].blue;
        green_sum += src[n-dim+1].green;
        red_sum   += src[n-dim+1].red;

        blue_sum  += src[n].blue;
        green_sum += src[n].green;
        red_sum   += src[n].red;

        blue_sum  += src[n+1].blue;
        green_sum += src[n+1].green;
        red_sum   += src[n+1].red;

        blue_sum  += src[n+dim].blue;
        green_sum += src[n+dim].green;
        red_sum   += src[n+dim].red;

        blue_sum  += src[n+dim+1].blue;
        green_sum += src[n+dim+1].green;
        red_sum   += src[n+dim+1].red;

        dst[n].blue  = blue_sum/6;
        dst[n].green = green_sum/6;
        dst[n].red   = red_sum/6;

    }

    // i = dim-1
    i = dim-1;
    n = (dim-1) * dim+1;
    for(j = 1; j < (dim-1); j++,n++){
        //dst[n]= new_avg(dim, i, j, src);
        //n++;
        if(flt[n]){
            dst[n] = src[n];
            continue;
        }
        blue_sum  = src[n-dim-1].blue;
        green_sum = src[n-dim-1].green;
        red_sum   = src[n-dim-1].red;

        blue_sum  += src[n-dim].blue;
        green_sum += src[n-dim].green;
        red_sum   += src[n-dim].red;

        blue_sum  += src[n-dim+1].blue;
        green_sum += src[n-dim+1].green;
        red_sum   += src[n-dim+1].red;

        blue_sum  += src[n-1].blue;
        green_sum += src[n-1].green;
        red_sum   += src[n-1].red;

        blue_sum  += src[n].blue;
        green_sum += src[n].green;
        red_sum   += src[n].red;

        blue_sum  += src[n+1].blue;
        green_sum += src[n+1].green;
        red_sum   += src[n+1].red;

        dst[n].blue  = blue_sum/6;
        dst[n].green = green_sum/6;
        dst[n].red   = red_sum/6;




    }

    // j = dim-1 case
    j = dim-1;
    n = dim+dim-1;
    for(i = 1; i < (dim-1); i++){
        //dst[n] = new_avg(dim,i,j,src);
        //n+=dim;
        if(flt[n]){
            dst[n] = src[n];
            continue;
        }
        blue_sum  = src[n-dim].blue;
        green_sum = src[n-dim].green;
        red_sum   = src[n-dim].red;

        blue_sum  += src[n-dim-1].blue;
        green_sum += src[n-dim-1].green;
        red_sum   += src[n-dim-1].red;

        blue_sum  += src[n].blue;
        green_sum += src[n].green;
        red_sum   += src[n].red;

        blue_sum  += src[n-1].blue;
        green_sum += src[n-1].green;
        red_sum   += src[n-1].red;

        blue_sum  += src[n+dim].blue;
        green_sum += src[n+dim].green;
        red_sum   += src[n+dim].red;

        blue_sum  += src[n+dim-1].blue;
        green_sum += src[n+dim-1].green;
        red_sum   += src[n+dim-1].red;

        dst[n].blue  = blue_sum/6;
        dst[n].green = green_sum/6;
        dst[n].red   = red_sum/6;

        n+=dim;
    }

}

//
//
// Alp's region ends
//
//

typedef struct {
    int red;
    int green;
    int blue;
} pixel_sum_oznakn;

pixel_sum_oznakn sum3v[1050000];
pixel_sum_oznakn sum3h[1050000];
char oznakn_bokeh_descr[] = "oznakn_bokeh";
void oznakn_bokeh(int dim, pixel *src, short *flt, pixel *dst)
{
    unsigned int i, j, k;
    int dim2 = dim*dim;
    int dimdim = 2*dim;
    pixel_sum_oznakn start;


    /* top left */
    k = 0;
    if (flt[k]) dst[k] = src[k];
    else {
        dst[k].red   = ((int) src[k].red   + (int) src[k + 1].red   + (int) src[k + dim].red   + (int) src[k + dim + 1].red) / 4;
        dst[k].green = ((int) src[k].green + (int) src[k + 1].green + (int) src[k + dim].green + (int) src[k + dim + 1].green) / 4;
        dst[k].blue  = ((int) src[k].blue  + (int) src[k + 1].blue  + (int) src[k + dim].blue  + (int) src[k + dim + 1].blue) / 4;
    }

    /* top right */
    k = dim-1;
    if (flt[k]) dst[k] = src[k];
    else {
        dst[k].red   = ((int) src[k].red   + (int) src[k - 1].red   + (int) src[k + dim].red   + (int) src[k + dim - 1].red) / 4;
        dst[k].green = ((int) src[k].green + (int) src[k - 1].green + (int) src[k + dim].green + (int) src[k + dim - 1].green) / 4;
        dst[k].blue  = ((int) src[k].blue  + (int) src[k - 1].blue  + (int) src[k + dim].blue  + (int) src[k + dim - 1].blue) / 4;
    }

    /* bottom left */
    k = dim2-dim;
    if (flt[k]) dst[k] = src[k];
    else {
        dst[k].red   = ((int) src[k].red   + (int) src[k + 1].red   + (int) src[k - dim].red   + (int) src[k - dim + 1].red) / 4;
        dst[k].green = ((int) src[k].green + (int) src[k + 1].green + (int) src[k - dim].green + (int) src[k - dim + 1].green) / 4;
        dst[k].blue  = ((int) src[k].blue  + (int) src[k + 1].blue  + (int) src[k - dim].blue  + (int) src[k - dim + 1].blue) / 4;
    }

    /* bottom right */
    k = dim2-1;
    if (flt[k]) dst[k] = src[k];
    else {
        dst[k].red   = ((int) src[k].red   + (int) src[k - 1].red   + (int) src[k - dim].red   + (int) src[k - dim - 1].red) / 4;
        dst[k].green = ((int) src[k].green + (int) src[k - 1].green + (int) src[k - dim].green + (int) src[k - dim - 1].green) / 4;
        dst[k].blue  = ((int) src[k].blue  + (int) src[k - 1].blue  + (int) src[k - dim].blue  + (int) src[k - dim - 1].blue) / 4;
    }



    for (i = 0; i < dimdim; i += dim) {
        start.red   = (int) src[i].red   + (int) src[i + 1].red   + (int) src[i + 2].red;
        start.green = (int) src[i].green + (int) src[i + 1].green + (int) src[i + 2].green;
        start.blue  = (int) src[i].blue  + (int) src[i + 1].blue  + (int) src[i + 2].blue;

        sum3h[i + 1] = start;

        for (j = i + 1; j < i + dim - 2; j++) {
            start.red   += (int) src[j + 2].red   - (int) src[j - 1].red;
            start.green += (int) src[j + 2].green - (int) src[j - 1].green;
            start.blue  += (int) src[j + 2].blue  - (int) src[j - 1].blue;

            sum3h[j + 1] = start;
        }
    }

    for (i = dim2-dimdim; i < dim2; i += dim) {
        start.red   = (int) src[i].red   + (int) src[i + 1].red   + (int) src[i + 2].red;
        start.green = (int) src[i].green + (int) src[i + 1].green + (int) src[i + 2].green;
        start.blue  = (int) src[i].blue  + (int) src[i + 1].blue  + (int) src[i + 2].blue;

        sum3h[i + 1] = start;

        for (j = i + 1; j < i + dim - 2; j++) {
            start.red   += (int) src[j + 2].red   - (int) src[j - 1].red;
            start.green += (int) src[j + 2].green - (int) src[j - 1].green;
            start.blue  += (int) src[j + 2].blue  - (int) src[j - 1].blue;

            sum3h[j + 1] = start;
        }
    }

    for (j = 0; j < dim; j++) {
        start.red   = (int) src[j].red   + (int) src[j + dim].red   + (int) src[j + dimdim].red;
        start.green = (int) src[j].green + (int) src[j + dim].green + (int) src[j + dimdim].green;
        start.blue  = (int) src[j].blue  + (int) src[j + dim].blue  + (int) src[j + dimdim].blue;

        sum3v[j + dim] = start;

        for (i = j + dim; i < j + dim*(dim - 2); i += dim) {
            start.red   += (int) src[i + dimdim].red   - (int) src[i - dim].red;
            start.green += (int) src[i + dimdim].green - (int) src[i - dim].green;
            start.blue  += (int) src[i + dimdim].blue  - (int) src[i - dim].blue;

            sum3v[i + dim] = start;
        }
    }



    /* top bottom */
    for (i = 1; i < dim - 1; i++) {
        if (flt[i]) dst[i] = src[i];
        else {
            dst[i].red   = (sum3h[i].red   + sum3h[i + dim].red) / 6;
            dst[i].green = (sum3h[i].green + sum3h[i + dim].green) / 6;
            dst[i].blue  = (sum3h[i].blue  + sum3h[i + dim].blue) / 6;
        }

        k = i + dim2 - dim;
        if (flt[k]) dst[k] = src[k];
        else {
            dst[k].red   = (sum3h[k].red   + sum3h[k - dim].red) / 6;
            dst[k].green = (sum3h[k].green + sum3h[k - dim].green) / 6;
            dst[k].blue  = (sum3h[k].blue  + sum3h[k - dim].blue) / 6;
        }
    }

    /* left right */
    for (i = dim; i < dim2 - dim; i += dim) {
        if (flt[i]) dst[i] = src[i];
        else {
            dst[i].red   = (sum3v[i].red   + sum3v[i + 1].red) / 6;
            dst[i].green = (sum3v[i].green + sum3v[i + 1].green) / 6;
            dst[i].blue  = (sum3v[i].blue  + sum3v[i + 1].blue) / 6;
        }

        k = i + dim - 1;
        if (flt[k]) dst[k] = src[k];
        else {
            dst[k].red   = (sum3v[k].red   + sum3v[k - 1].red) / 6;
            dst[k].green = (sum3v[k].green + sum3v[k - 1].green) / 6;
            dst[k].blue  = (sum3v[k].blue  + sum3v[k - 1].blue) / 6;
        }
    }

    /* generic part */
    for (i = dim + 1; i < dim*(dim - 1) + 1; i += dim) {
        start.red   = sum3v[i - 1].red   + sum3v[i].red   + sum3v[i + 1].red;
        start.green = sum3v[i - 1].green + sum3v[i].green + sum3v[i + 1].green;
        start.blue  = sum3v[i - 1].blue  + sum3v[i].blue  + sum3v[i + 1].blue;

        if (flt[i]) dst[i] = src[i];
        else {
            dst[i].red   = start.red / 9;
            dst[i].green = start.green / 9;
            dst[i].blue  = start.blue / 9;
        }

        for (j = i + 1; j < i + dim - 2; j++) {
            start.red   += sum3v[j + 1].red   - sum3v[j - 2].red;
            start.green += sum3v[j + 1].green - sum3v[j - 2].green;
            start.blue  += sum3v[j + 1].blue  - sum3v[j - 2].blue;

            if (flt[j]) dst[j] = src[j];
            else {
                dst[j].red   = start.red / 9;
                dst[j].green = start.green / 9;
                dst[j].blue  = start.blue / 9;
            }
        }
    }
}

/*********************************************************************
 * register_bokeh_functions - Register all of your different versions
 *     of the bokeh kernel with the driver by calling the
 *     add_bokeh_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_bokeh_functions()
{
    add_bokeh_function(&naive_bokeh, naive_bokeh_descr);
    add_bokeh_function(&bokeh, bokeh_descr);
    /* ... Register additional test functions here */
    add_bokeh_function(&alp_bokeh,alp_bokeh_descr);
    add_bokeh_function(&ucanyiit_bokeh,ucanyiit_bokeh_descr);
    add_bokeh_function(&oznakn_bokeh,oznakn_bokeh_descr);
}

/***************************
 * HADAMARD PRODUCT KERNEL *
 ***************************/

/******************************************************
 * Your different versions of the hadamard product functions go here
 ******************************************************/

/*
 * naive_hadamard - The naive baseline version of hadamard product of two matrices
 */
char naive_hadamard_descr[] = "naive_hadamard The naive baseline version of hadamard product of two matrices";
void naive_hadamard(int dim, int *src1, int *src2, int *dst) {

    int i, j;

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = src1[RIDX(i, j, dim)] * src2[RIDX(i, j, dim)];
}

/*
 * hadamard - Your current working version of hadamard product
 * IMPORTANT: This is the version you will be graded on
 */
char hadamard_descr[] = "hadamard: Current working version";
void hadamard(int dim, int *src1, int *src2, int *dst)
{
    naive_hadamard(dim, src1, src2, dst);
}
/*********************************************************************
 * register_hadamard_functions - Register all of your different versions
 *     of the hadamard kernel with the driver by calling the
 *     add_hadamard_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_hadamard_functions()
{
    add_hadamard_function(&naive_hadamard, naive_hadamard_descr);
    add_hadamard_function(&hadamard, hadamard_descr);
    /* ... Register additional test functions here */
}

