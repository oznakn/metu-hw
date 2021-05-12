/*********************************************************
 * config.h - Configuration data for the driver.c program.
 *********************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* 
 * CPEs for the baseline (naive) version of the bokeh function that
 * was handed out to the students. Rd is the measured CPE for a dxd
 * image. Run the driver.c program on your system to get these
 * numbers.  
 */
#define B32    54.5
#define B64    54.5
#define B128   54.0
#define B256   54.5
#define B512   54.7

/* 
 * CPEs for the baseline (naive) version of the smooth function that
 * was handed out to the students. Sd is the measure CPE for a dxd
 * image. Run the driver.c program on your system to get these
 * numbers.  
 */
#define H64   	1.8
#define H128  	1.7
#define H256  	1.6
#define H512  	1.6
#define H1024 	2.2

#endif /* _CONFIG_H_ */
