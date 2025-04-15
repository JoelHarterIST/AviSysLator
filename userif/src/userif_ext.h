/// \file userif_ext.h
/// \brief This file contains the user interface extension.

#ifndef _USERIF_EXT_H
#define _USERIF_EXT_H

#include "common.h"

#define LOG_NAME_SIZE (64)
#define INIT_DATA_MAX (256)
#define TIME_SERIES_LOG_DATA_MAX (256)
#define CYCLIC_LOG_DATA_MAX (256)
#define EVENT_LOG_DATA_MAX (256)
#define EVENT_MONITOR_DATA_MAX (16)

#define MONTE_CARLO_DATA_MAX (256)
#define DELAY_DATA_MAX (256)
#define DELAY_BUF_MAX (128)

#define INPUT_FILE_NAME_SIZE (64)

typedef enum {
	OPT_TYPE_NONE = 0, 	///< defalut 
	OPT_TYPE_CHG, 		///< change
	OPT_TYPE_LTE, 		///< less than or equal
	OPT_TYPE_LT, 		///< less than
	OPT_TYPE_GTE, 		///< greater than or equal
	OPT_TYPE_GT, 		///< greater than
	OPT_TYPE_EQ, 		///< equal
	OPT_TYPE_NEQ,		///< not equal
} option_type_e;

typedef enum {
	EVAL_NONE = 0, 	///< default
	EVAL_FALSE, 	///< event condition is false
	EVAL_TRUE, 		///< event condition is true
} eval_type_e;

typedef enum {
	LOG_TYPE_UNKNOWN = 0, ///< default
	LOG_TYPE_INT, 		  ///< int
	LOG_TYPE_DBL, 		  ///< double
} log_type_e;


typedef struct  {
	const char *str; 
	option_type_e type;     
} option_type_t;


typedef struct {
	int time_series_log_cycle; ///< time series log cycle
	int std_out_log_cycle; 	   ///< stdout log cycle
} user_interface_params_t;

typedef struct {
	char name[LOG_NAME_SIZE]; ///< variable name
	ADDRESS_T adr; 			  ///< variable address
	log_type_e type; 		  ///< variable type
} variable_data_t;


typedef struct {
	variable_data_t v;  ///< variable data
	ADDRESS_T next; 	///< next address
} time_series_log_data_t;

typedef struct {
	variable_data_t v;  ///< variable data
	ADDRESS_T next; 	///< next address 
} cyclic_log_data_t;

typedef struct {
	variable_data_t v;  ///< variable data
	ADDRESS_T next; 	///< next address
} event_monitor_data_t;


union Value{
	int32_t i_value; ///< int value
	double  d_value; ///< double value
};

typedef struct {
	variable_data_t v; 		///< variable data
	union Value thr; 		///< threshold value
	union Value prv; 		///< previous value
	option_type_e opt_type; ///< event option type
	eval_type_e eval_type; 	///< evaluation result
	ADDRESS_T next;

	event_monitor_data_t evmd[EVENT_MONITOR_DATA_MAX]; ///< event monitor data 
} event_log_data_t;

extern user_interface_params_t uip;
extern time_series_log_data_t tsld[TIME_SERIES_LOG_DATA_MAX];
extern cyclic_log_data_t sold[CYCLIC_LOG_DATA_MAX];
extern event_log_data_t evld[EVENT_LOG_DATA_MAX];

#endif