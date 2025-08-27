/// \file symbol_map_yaml.h
/// \brief This file contains the symbol map yaml schema.

#ifndef __SYMBOL_MAP_YAML_H__
#define __SYMBOL_MAP_YAML_H__

#include "common.h"
#include <cyaml/cyaml.h>

#define SYMBOL_MAP_FILE_NAME "symbol_map.yaml"

// Enumeration
typedef enum {
	SYMBOL_TYPE_INT = 1,
	SYMBOL_TYPE_DBL,
	SYMBOL_TYPE_VECTOR,
	SYMBOL_TYPE_QUAT,
	SYMBOL_TYPE_MATRIX,
} symbol_type_e;

///////////////////////////////////////////////////////
// cyaml data struct
///////////////////////////////////////////////////////
//-----------------------------------------------------
// data schema
//-----------------------------------------------------
typedef struct {
	const char *name;
	ADDRESS_T address;
	symbol_type_e type;
} symbol_data_t;

//-----------------------------------------------------
// file schema
//-----------------------------------------------------
typedef struct {
	const char *title;
	ADDRESS_T mark_address;
	symbol_data_t *symbols;
	uint32_t symbols_count;
} symbol_map_t;

//-----------------------------------------------------
// symbol map data schema
//-----------------------------------------------------
static const cyaml_strval_t symbol_type_strings[] = {
	{ "int32_t",   SYMBOL_TYPE_INT },
	{ "double",  SYMBOL_TYPE_DBL },
	{ "vec3_t",  SYMBOL_TYPE_VECTOR },
	{ "quat_t",  SYMBOL_TYPE_QUAT },
	{ "mat33_t",  SYMBOL_TYPE_MATRIX },
};

static const cyaml_schema_field_t symbol_data_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
			"name", CYAML_FLAG_POINTER,
			symbol_data_t, name, 0, CYAML_UNLIMITED),

	CYAML_FIELD_UINT(
			"address", CYAML_FLAG_DEFAULT,
			symbol_data_t, address),

	CYAML_FIELD_ENUM(
			"type", CYAML_FLAG_DEFAULT,
			symbol_data_t, type, symbol_type_strings,
			CYAML_ARRAY_LEN(symbol_type_strings)),

	CYAML_FIELD_END
};

static const cyaml_schema_value_t symbol_data_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
			symbol_data_t, symbol_data_fields_schema),
};

//-----------------------------------------------------
// symbol map file schema
//-----------------------------------------------------
static const cyaml_schema_field_t symbol_map_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
			"title", CYAML_FLAG_POINTER,
			symbol_map_t, title, 0, CYAML_UNLIMITED),

	CYAML_FIELD_UINT(
			"mark_address", CYAML_FLAG_DEFAULT,
			symbol_map_t, mark_address),

	CYAML_FIELD_SEQUENCE(
			"symbols", CYAML_FLAG_POINTER,
			symbol_map_t, symbols,
			&symbol_data_schema, 0, CYAML_UNLIMITED),

	CYAML_FIELD_IGNORE("description", CYAML_FLAG_OPTIONAL),

	CYAML_FIELD_END
};

static const cyaml_schema_value_t symbol_map_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER,
			symbol_map_t, symbol_map_fields_schema),
};

#endif
