#ifndef __GUITRNS_H__
#define __GUITRNS_H__

#include "common.h"
#include "gncmath.h"
#include "guitrns_ext.h"
#include "ptrns_ext.h"

int guitrns_init();
int guitrns_main(ptrns_state_t* ppts, guitrns_state_t* pgts);
int guitrns_core(int init, vec3_t r_, vec3_t v_, guitrns_state_t* pgts);

#endif
