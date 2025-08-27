//  @file mdlrand.c
//  @brief This file contains the random number generator functions for model.
#ifndef  __MDL_RAND_H__
#define  __MDL_RAND_H__

#include "gncmath.h"

#ifdef __cplusplus
extern "C" {
#endif

void mdlrand_init(unsigned long seed);
uint32_t mdlrand_gen_uniform_u32();
double mdlrand_gen_uniform_dbl();
double mdlrand_gen_normal_dbl(double mean, double stddev);
vec3_t mdlrand_gen_normal_v3(vec3_t v, vec3_t stddev_);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
