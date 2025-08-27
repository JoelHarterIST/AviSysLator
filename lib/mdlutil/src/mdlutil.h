/// @file mdlutil.h
/// @brief This file contains the utility functions for model.

#ifndef __MDL_UTIL_H__
#define __MDL_UTIL_H__

#include "gncmath.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t mdlutil_delay_i32(int32_t* array, uint32_t size, uint32_t ind, int32_t* src_adr, int32_t* dst_adr);
uint32_t mdlutil_delay_dbl(double* array, uint32_t size, uint32_t ind, double* src_adr, double* dst_adr);
uint32_t mdlutil_delay_v3(vec3_t* array, uint32_t size, uint32_t ind, vec3_t* src_adr, vec3_t* dst_adr);

#ifdef __cplusplus
} // extern "C"
#endif

#endif