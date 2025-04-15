/// \file test_case_yaml.h
/// \brief This file contains the test case yaml schema.

#ifndef _TEST_CASE_YAML_H
#define _TEST_CASE_YAML_H

#include "common.h"
#include "common_yaml.h"
#include <cyaml/cyaml.h>

///////////////////////////////////////////////////////
// cyaml data struct
///////////////////////////////////////////////////////
//-----------------------------------------------------
// data schema
//-----------------------------------------------------
typedef struct {
	const char *variable;
	const char *value;
} init_setting_t;

typedef struct {
	const char *variable;
	option_type_e opt_type;
	const char *threshold;
	uint32_t monitors_count;
	const char ** monitors;
} event_log_t;

typedef struct {
	uint32_t cycle;
	const char **input_files;
	uint32_t input_files_count;
	const char *output_file;
	uint32_t cyclic_log_vairables_count;
	const char **cyclic_log_vairables;
	event_log_t * event_logs;
	uint32_t event_logs_count;
} std_out_log_t;

typedef struct {
	uint32_t cycle;
	const char **input_files;
	uint32_t input_files_count;
	const char *output_file;
	const char **time_series_log_variables;
	uint32_t time_series_log_variables_count;
} time_series_log_t;


typedef struct {
	const char **input_files;
	uint32_t input_files_count;
	init_setting_t *init_settings;
	uint32_t init_settings_count;
} init_t;


//-----------------------------------------------------
// file schema
//-----------------------------------------------------
typedef struct {
	const char *title;
	init_t *init;
	std_out_log_t* std_out_log;
	time_series_log_t *time_series_log;
} test_case_t;

typedef struct {
	const char *title;    
	init_setting_t *init_setting;
	uint32_t init_setting_count;
} init_input_t;

typedef struct {
	const char *title;
	const char **time_series_log_variables;
	uint32_t time_series_log_variables_count;
} time_series_log_input_t;

typedef struct {
	const char *title;
	uint32_t cyclic_log_vairables_count;
	const char **cyclic_log_vairables;
	event_log_t * event_logs;
	uint32_t event_logs_count;
} std_out_log_input_t;

///////////////////////////////////////////////////////
// cyaml schema struct
///////////////////////////////////////////////////////
//-----------------------------------------------------
// init setting schema
//-----------------------------------------------------
static const cyaml_schema_field_t init_setting_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
			"variable", CYAML_FLAG_POINTER,
			init_setting_t, variable, 0, CYAML_UNLIMITED),

	CYAML_FIELD_STRING_PTR(
			"value", CYAML_FLAG_POINTER,
			init_setting_t, value, 0, CYAML_UNLIMITED),

	CYAML_FIELD_END
};

static const cyaml_schema_value_t init_setting_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
		init_setting_t, init_setting_fields_schema),
};

//-----------------------------------------------------
// event log schema
//-----------------------------------------------------
static const cyaml_strval_t option_type_strings[] = {
	{ "CHG",   OPT_TYPE_CHG },
	{ "LTE",  OPT_TYPE_LTE },
	{ "LT",  OPT_TYPE_LT },	
	{ "GTE",  OPT_TYPE_GTE },	
	{ "GT",  OPT_TYPE_GT },	
	{ "EQ",  OPT_TYPE_EQ },	
	{ "NEQ",  OPT_TYPE_NEQ },	
};

static const cyaml_schema_field_t event_log_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
			"variable", CYAML_FLAG_POINTER,
			event_log_t, variable, 0, CYAML_UNLIMITED),


	CYAML_FIELD_ENUM(
			"option", CYAML_FLAG_DEFAULT,
			event_log_t, opt_type, option_type_strings,
			CYAML_ARRAY_LEN(option_type_strings)),			

	CYAML_FIELD_STRING_PTR(
			"threshold", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
			event_log_t, threshold, 0, CYAML_UNLIMITED),

	CYAML_FIELD_SEQUENCE(
			"monitor", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
			event_log_t, monitors,
			&string_ptr_schema, 0, CYAML_UNLIMITED),			


	CYAML_FIELD_END
};

static const cyaml_schema_value_t event_log_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
			event_log_t, event_log_fields_schema),
};


//-----------------------------------------------------
// time series schema
//-----------------------------------------------------
static const cyaml_schema_field_t time_series_log_fields_schema[] = {
	CYAML_FIELD_UINT(
			"cycle", CYAML_FLAG_DEFAULT,
			time_series_log_t, cycle),

	CYAML_FIELD_SEQUENCE(
			"input_file", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
			time_series_log_t, input_files,
			&string_ptr_schema, 0, CYAML_UNLIMITED),		

	CYAML_FIELD_STRING_PTR(
			"output_file", CYAML_FLAG_POINTER,
			time_series_log_t, output_file, 0, CYAML_UNLIMITED),

	CYAML_FIELD_SEQUENCE(
			"time_series_log_variables", CYAML_FLAG_POINTER,
			time_series_log_t, time_series_log_variables,
			&string_ptr_schema, 0, CYAML_UNLIMITED),		
	CYAML_FIELD_END
};

//-----------------------------------------------------
// std out schema
//-----------------------------------------------------
static const cyaml_schema_field_t std_out_log_fields_schema[] = {
	CYAML_FIELD_UINT(
			"cycle", CYAML_FLAG_DEFAULT,
			std_out_log_t, cycle),

	CYAML_FIELD_SEQUENCE(
			"input_file", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
			std_out_log_t, input_files,
			&string_ptr_schema, 0, CYAML_UNLIMITED),

	CYAML_FIELD_STRING_PTR(
			"output_file", CYAML_FLAG_POINTER,
			std_out_log_t, output_file, 0, CYAML_UNLIMITED),

	CYAML_FIELD_SEQUENCE(
			"cyclic_log_variables", CYAML_FLAG_POINTER,
			std_out_log_t, cyclic_log_vairables,
			&string_ptr_schema, 0, CYAML_UNLIMITED),			

	CYAML_FIELD_SEQUENCE(
			"event_log", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
			std_out_log_t, event_logs, 
			&event_log_schema, 0, CYAML_UNLIMITED),

	CYAML_FIELD_END
};


//-----------------------------------------------------
// init schema
//-----------------------------------------------------
static const cyaml_schema_field_t init_fields_schema[] = {
	CYAML_FIELD_SEQUENCE(
			"input_file", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
			init_t, input_files,
			&string_ptr_schema, 0, CYAML_UNLIMITED),			

	CYAML_FIELD_SEQUENCE(
			"settings", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
			init_t, init_settings, 
			&init_setting_schema, 0, CYAML_UNLIMITED),

	CYAML_FIELD_END
};


//-----------------------------------------------------------------
// test_case file schema
//-----------------------------------------------------------------
static const cyaml_schema_field_t test_case_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
			"title", CYAML_FLAG_POINTER,
			test_case_t, title, 0, CYAML_UNLIMITED),

	CYAML_FIELD_MAPPING_PTR(
			"init", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
			test_case_t, init, init_fields_schema),			

	CYAML_FIELD_MAPPING_PTR(
			"time_series_log", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
			test_case_t, time_series_log, time_series_log_fields_schema),

	CYAML_FIELD_MAPPING_PTR(
			"std_out_log", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
			test_case_t, std_out_log, std_out_log_fields_schema),

	CYAML_FIELD_IGNORE("description", CYAML_FLAG_OPTIONAL),

	CYAML_FIELD_END
};

static const cyaml_schema_value_t test_case_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER,
			test_case_t, test_case_fields_schema),
};

//-----------------------------------------------------------------
// init input file schema
//-----------------------------------------------------------------
static const cyaml_schema_field_t init_input_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
			"title", CYAML_FLAG_POINTER,
			init_input_t, title, 0, CYAML_UNLIMITED),

	CYAML_FIELD_SEQUENCE(
			"settings", CYAML_FLAG_POINTER,
			init_input_t, init_setting, 
			&init_setting_schema, 0, CYAML_UNLIMITED),

    CYAML_FIELD_IGNORE("description", CYAML_FLAG_OPTIONAL),

	CYAML_FIELD_END
};

static const cyaml_schema_value_t init_input_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER,
                init_input_t, init_input_fields_schema),
};

//-----------------------------------------------------------------
// time series log_input file schema
//-----------------------------------------------------------------
static const cyaml_schema_field_t time_series_log_input_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
			"title", CYAML_FLAG_POINTER,
			time_series_log_input_t, title, 0, CYAML_UNLIMITED),

	CYAML_FIELD_SEQUENCE(
			"time_series_log_variables", CYAML_FLAG_POINTER,
			time_series_log_input_t, time_series_log_variables,
			&string_ptr_schema, 0, CYAML_UNLIMITED),

	CYAML_FIELD_IGNORE("description", CYAML_FLAG_OPTIONAL),

	CYAML_FIELD_END
};

static const cyaml_schema_value_t time_series_log_input_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER,
			time_series_log_input_t, time_series_log_input_fields_schema),
};

//-----------------------------------------------------------------
// std out log input file schema
//-----------------------------------------------------------------
static const cyaml_schema_field_t std_out_log_input_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
			"title", CYAML_FLAG_POINTER,
			std_out_log_input_t, title, 0, CYAML_UNLIMITED),

	CYAML_FIELD_SEQUENCE(
			"cyclic_log_variables", CYAML_FLAG_POINTER,
			std_out_log_input_t, cyclic_log_vairables,
			&string_ptr_schema, 0, CYAML_UNLIMITED),

    CYAML_FIELD_SEQUENCE(
            "event_log", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL,
            std_out_log_input_t, event_logs, 
            &event_log_schema, 0, CYAML_UNLIMITED),            

	CYAML_FIELD_IGNORE("description", CYAML_FLAG_OPTIONAL),

	CYAML_FIELD_END
};

static const cyaml_schema_value_t std_out_log_input_schema = {
	CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER,
			std_out_log_input_t, std_out_log_input_fields_schema),
};

///////////////////////////////////////////////////////

#endif
