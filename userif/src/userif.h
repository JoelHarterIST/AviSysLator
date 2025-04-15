/// \file userif.h
/// \brief This file contains the user interface.

#ifndef _USERIF_H
#define _USERIF_H

#include "common.h"
#include "userif_ext.h"
#include "symbol_map_yaml.h"
#include "test_case_yaml.h"
#include <cyaml/cyaml.h>


static const cyaml_config_t config = {
	.log_fn = cyaml_log,            /* Use the default logging function. */
	.mem_fn = cyaml_mem,            /* Use the default memory allocator. */
	.log_level = CYAML_LOG_WARNING, /* Logging errors and warnings only. */
};

int userif_init(double t, char* test_case_file);

int userif_main_loop_start(double t);

int userif_main_loop_end();

int userif_main(int cycle, double t);

int userif_finish(void);

#endif
