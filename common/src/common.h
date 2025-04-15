#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#if __SIZEOF_POINTER__ == 4
#define ADDRESS_T uint32_t
#else
#define ADDRESS_T uint64_t
#endif

// EXIT STATUS
#define	EXIT_SUCCESS	0	/* Successful exit status. 0 <= */
#define	EXIT_FAILURE	-1	/* Failing exit status.  0 >  */
#define	EXIT_OK	0	/* Successful exit status. 0 <= */
#define	EXIT_NG	-1	/* Failing exit status.  0 >  */

// MATH CONSTANTS
#define PI (3.14159265358979323846)
#define DEG_RAD (PI/180.0)
#define RAD_DEG (180.0/PI)

// NATURE CONSTANTS
#define MU_E (3.986004418E+14) // [m^3 s^-2]
#define G_E (9.80665) // [m/s^2]


#endif
