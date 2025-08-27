#ifndef _MAIN_EXT_H
#define _MAIN_EXT_H

#include "common.h"

typedef struct {
	double dt;
	int32_t num_of_loop;
} main_param_t;

typedef struct {
	double t;
} main_state_t;

extern main_param_t mp;
extern main_state_t ms;

#endif
