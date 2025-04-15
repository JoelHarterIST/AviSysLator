#ifndef _PLANT_H
#define _PLANT_H

#include "common.h"
#include "plant_ext.h"

#define LEN_OF_STATE (6)

int eq_of_system(plant_params_t* pp, double* x, double* dx, double* u);
int plant_main(double* t, double dt, plant_state_t* ps);

#endif
