/// \file common_yaml.h
/// \brief This file contains the common yaml schema.

#ifndef __COMMON_YAML_H__
#define __COMMON_YAML_H__

#include "common.h"
#include <cyaml/cyaml.h>

///////////////////////////////////////////////////////
// cyaml schema struct
///////////////////////////////////////////////////////
//-----------------------------------------------------
// string ptr schema
//-----------------------------------------------------
static const cyaml_schema_value_t string_ptr_schema = {
	CYAML_VALUE_STRING(CYAML_FLAG_POINTER, char, 0, CYAML_UNLIMITED),
};

#endif
