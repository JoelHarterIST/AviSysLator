#ifndef _GUIDANCE_H
#define _GUIDANCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "gncmath.h"
#include "guidance_ext.h"
#include "plant_ext.h"

int guidance_init();
int guidance_main(plant_state_t* ps, guidance_state_t* gs);
int guidance_core(int init, vec3_t r_, vec3_t v_, guidance_state_t* gs);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
