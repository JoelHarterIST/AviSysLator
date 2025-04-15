#ifndef _PLANT_EXT_H
#define _PLANT_EXT_H

#include "gncmath.h"

typedef struct {
	double isp;
} plant_params_t;

typedef struct {
	vec3_t pos_;
	vec3_t vel_;
	vec3_t acl_;
} plant_state_t;

extern plant_params_t pp;
extern plant_state_t ps;

#endif
