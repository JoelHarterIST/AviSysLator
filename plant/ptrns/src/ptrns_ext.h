#ifndef __PTRNS_EXT_H__
#define __PTRNS_EXT_H__

#include "gncmath.h"

typedef struct {
	double isp;
} ptrns_param_t;

typedef struct {
	vec3_t pos_;
	vec3_t vel_;
	vec3_t acl_;
} ptrns_state_t;

extern ptrns_param_t ptp;
extern ptrns_state_t pts;

#endif
