#include<stdio.h>
FILE *f;
int n=3, m=7, b[3][7];
unsigned int q=882334423;
float ff=3.1415926, a[10];
double s, c[3];
long x=1234567901011;
main()
{ int i,j;
f = fopen("sample.dat","w");
for(i=1, a[0]=1.5; i<10; i++) a[i] = 1.5*a[i-1];
for(i=1; i<4; i++) c[i-1] = ((double)i*10)/7.0 + i*i;
for(i=0; i<3; i++) for (j=0; j<7; j++) b[i][j] = 200 - (i*j*100+i+j);
fwrite(&n,sizeof(int),1,f);
fwrite(&ff,sizeof(float),1,f);
fwrite(&m,sizeof(int),1,f);
fwrite(a,sizeof(float),10,f);
fwrite(c,sizeof(double),n,f);
fwrite(&x,sizeof(long),1,f);
fwrite(&q,sizeof(unsigned int),1,f);
fwrite(b,sizeof(int),n*m,f);
fwrite(&s,sizeof(double),1,f);
fclose(f); }
