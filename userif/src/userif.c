/// \file userif.c
/// \brief This file contains the user interface functions.

#include "userif.h"
#include "gncmath.h"
#include <errno.h>
#include <unistd.h>
#include <linux/limits.h>
#define STD_OUT_LOG_TEMP_FILE_NAME "std_log_out_temp.txt" ///< std out log temp file name

int32_t mark_variable = 0; ///< mark variable for calculating the base address of the symbol map

user_interface_param_t uip = {0}; 							 ///< user interface parameters
time_series_log_data_t tsld[TIME_SERIES_LOG_DATA_MAX] = {0}; ///< time series log data array
cyclic_log_data_t cold[CYCLIC_LOG_DATA_MAX] = {0}; 			 ///< cyclic log data array
event_log_data_t evld[EVENT_LOG_DATA_MAX] = {0}; 			 ///< event log data array
char init_vars[INIT_DATA_MAX][LOG_NAME_SIZE]={0}; 			 ///< initialized variables

FILE* tslfp = NULL;	///< time series log file pointer
FILE* solfp = NULL; ///< std out log file pointer
char std_log_out_temp_file_path[PATH_MAX] = {0}; 	///< std out log temp file path
char std_out_log_file_path[PATH_MAX] = {0}; 		///< std out log file path

#define LTE(val, thr) (thr >= val)
#define LT(val, thr) (thr >  val)
#define GTE(val, thr) (thr <= val)
#define GT(val, thr) (thr <  val)
#define EQ(src, dest) (src == dest)
#define NEQ(src, dest) (src != dest)
#define CHG(src, dest) (src != dest)

/// @fn int std_trace_init(char* trace_file_path)
/// @brief Initialize trace log file
/// @param[in] trace_file_path: log file path
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
int std_trace_init(char* trace_file_path) {
    // open std out log file
	solfp = fopen(trace_file_path, "w");
	if (solfp == NULL) {
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		return EXIT_FAILURE;
	}
    return EXIT_SUCCESS;
}

/// @brief Output log stream to stdout and log file.
/// @param[in] format: format of log output
void std_out_trace(char *format, ...) {
    char buf[255] = {0};
    va_list args;
 
    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);
#ifdef	AVISYSLATOR_GNC	
    printf("%s", (char*)buf);
#endif	
    if (solfp != NULL) {
        fprintf(solfp, "%s", buf);
        fflush(solfp);
    }
}
 
/// @brief Output log stream to stderr and log file.
/// @param[in] format: format of log output
void stderr_out_trace(char *format, ...) {
    char buf[4096] = {0};
    va_list args;
    
    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);
#ifdef	AVISYSLATOR_GNC	
	fprintf(stderr, "%s", buf);
#endif	
    if (solfp != NULL) {
        fprintf(solfp, "%s", buf);
        fflush(solfp);
    }
 }
 
/// @brief Terminates trace log file.
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
int std_trace_term() {
    if (solfp != NULL) {
		fclose(solfp);
	    solfp = NULL;
	}
    return EXIT_SUCCESS;
 }


/// @brief Get the directory of the executable process.
/// @param[out] dir: directory of the executable file
/// @param[in] dir_len: length of the directory
void get_exe_dir(char* dir, int dir_len) {
	char exepath[1024]={0};
    int length = readlink("/proc/self/exe", exepath, sizeof(exepath) - 1);

    if (length != -1) {
		char* ret_ch = strrchr(exepath, '/');
		if (ret_ch != NULL) {
			*ret_ch = '\0';
		}
		// printf("exedir: %s\n", exepath);
		strncpy(dir, exepath, dir_len);
    } else {
        perror("readlink");
	}
}

/// @brief Get the address of the specified variable.
/// @param[in] symbol_map: symbol map of the target executable file 
/// @param [out] address: address of the target variable 
/// @param [out] type: type of the target variable 
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
int get_symbol_address(const char* _name_target, symbol_map_t* symbol_map, ADDRESS_T* address, symbol_type_e* type) {
	int i = 0;

	if (symbol_map == NULL || address == NULL || type == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	for (i=0; i < symbol_map->symbols_count; i++)	{
		if(0 == strcmp(_name_target, symbol_map->symbols[i].name)) {
			*address = symbol_map->symbols[i].address;
			*type = symbol_map->symbols[i].type;
			return EXIT_SUCCESS;
		}
	}

	return EXIT_FAILURE;
}

/// @brief Load symbol map file. 
/// @param[in] file_path: symbol map file path 
/// @param[in] p_adr_base: base address of the executable process
/// @param[out] pp_sm: symbol map data
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
int load_symbol_map(char* file_path, ADDRESS_T* p_adr_base, symbol_map_t** pp_sm) {
	int i = 0;
	ADDRESS_T address_mark = (ADDRESS_T)&mark_variable;
	cyaml_err_t _err;
	symbol_map_t* _psm = NULL;

	if (file_path == NULL || p_adr_base == NULL || pp_sm == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}	

	// load symbol map
	std_out_trace("load file: %s\n", file_path);
	_err = cyaml_load_file(file_path, &config,
			&symbol_map_schema, (void **) pp_sm, NULL);
	if (_err != CYAML_OK) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", cyaml_strerror(_err), __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// dump
	_psm = *pp_sm;
	// The "mark_variable" is special variable and it is used to calculate the base address of the symbol map.
	std_out_trace("title: %s\n", _psm->title);
	*p_adr_base = address_mark - _psm->mark_address;
	std_out_trace("abs address mark: 0x%016lx\n", address_mark);
	std_out_trace("map address mark: 0x%016lx\n", _psm->mark_address);
	std_out_trace("abs address base: 0x%016lx\n", *p_adr_base);
	std_out_trace("symbols:\n");
	for (i=0; i < _psm->symbols_count; i++) {
		std_out_trace("%u\t%s\t0x%08lx\t%d\n", i, _psm->symbols[i].name, _psm->symbols[i].address, _psm->symbols[i].type);
	}
	std_out_trace("---\n");

	return EXIT_SUCCESS;
}

/// @brief Load test case file. 
/// @param[in] file_path: test case file path 
/// @param[out] pp_tc: test case data
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
int load_test_case(char* file_path, test_case_t** pp_tc) {
	int i = 0, j = 0;
	cyaml_err_t _err;
	test_case_t* _ptc = NULL;

	if (file_path == NULL || pp_tc == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// load test case
	std_out_trace("load file: %s\n", file_path);
	_err = cyaml_load_file(file_path, &config,
			&test_case_schema, (void **) pp_tc, NULL);
	if (_err != CYAML_OK) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", cyaml_strerror(_err), __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// dump
	_ptc = *pp_tc;
	std_out_trace("title: %s\n", _ptc->title);
	if (_ptc->init != NULL) {
		std_out_trace("init:setting\n");
		for (i=0; i < _ptc->init->init_settings_count; i++) {
			std_out_trace("%u\t%s\t%s\n", i, _ptc->init->init_settings[i].variable, _ptc->init->init_settings[i].value);
		}
		std_out_trace("init:input_file\n");
		for (i=0; i < _ptc->init->input_files_count; i++) {
			std_out_trace("%u\t%s\n", i, _ptc->init->input_files[i]);
		}
	}
	
	if (_ptc->time_series_log != NULL) {
		std_out_trace("time series:log cycle: %d\n", _ptc->time_series_log->cycle);
		uip.time_series_log_cycle = _ptc->time_series_log->cycle;
		for (i=0; i < _ptc->time_series_log->input_files_count; i++) {
			std_out_trace("time series log input file: %s\n", _ptc->time_series_log->input_files[i]);
		}
		std_out_trace("time series log output file: %s\n", _ptc->time_series_log->output_file);
	}

	if (_ptc->std_out_log != NULL) {
		std_out_trace("std out log:input file count: %d\n", _ptc->std_out_log->input_files_count);
		std_out_trace("std out log:cycle: %d\n", _ptc->std_out_log->cycle);
		uip.std_out_log_cycle = _ptc->std_out_log->cycle;
		std_out_trace("std out log output file: %s\n", _ptc->std_out_log->output_file);

		std_out_trace("std out log:cyclic log variables:\n");
		for(i = 0; i < _ptc->std_out_log->cyclic_log_vairables_count; i++){
			std_out_trace("%u\t%s\n", i, _ptc->std_out_log->cyclic_log_vairables[i]);
		}

		std_out_trace("std out log:event log variables:\n");
		for (i=0; i < _ptc->std_out_log->event_logs_count; i++) {
			std_out_trace("%u\t%s\t%d\t%s\n", i, 
				_ptc->std_out_log->event_logs[i].variable, 
				_ptc->std_out_log->event_logs[i].opt_type, 
				_ptc->std_out_log->event_logs[i].threshold);

			std_out_trace("\trelation variables: %d\n", _ptc->std_out_log->event_logs[i].monitors_count);
			for(j=0; j < _ptc->std_out_log->event_logs[i].monitors_count; j++) {
				std_out_trace("\t%u\t%s\n", j, 
				_ptc->std_out_log->event_logs[i].monitors[j]);
			} 

		}	
	}
	std_out_trace("---\n");

	return EXIT_SUCCESS;
}

/// @brief Load input init file. 
/// @param[in] file_path: init input file path 
/// @param[out] pp_in: init input data
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
int load_init_input(char* file_path, init_input_t** pp_in) {
	int i = 0, j = 0;
	cyaml_err_t _err;
	init_input_t* _pin = NULL;

	if (file_path == NULL || pp_in == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// load init input
	std_out_trace("load file: %s\n", file_path);
	_err = cyaml_load_file(file_path, &config,
			&init_input_schema, (void **) pp_in, NULL);
	if (_err != CYAML_OK) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", cyaml_strerror(_err), __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// dump
	_pin = *pp_in;
	std_out_trace("title: %s\n", _pin->title);
	std_out_trace("init:setting\n");
	for (i=0; i < _pin->init_setting_count; i++) {
		std_out_trace("%u\t%s\t%s\n", i, _pin->init_setting[i].variable, _pin->init_setting[i].value);
	}
	std_out_trace("---\n");
}

/// @brief Load time series log input file.
/// @param[in] file_path: time series log input file path
/// @param[out] pp_tsli: time series log input data
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
int load_time_series_log_input(char* file_path, time_series_log_input_t** pp_tsli) {
	int i = 0;
	cyaml_err_t _err;
	time_series_log_input_t* _ptsli = NULL;

	if (file_path == NULL || pp_tsli == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// load time series log input
	std_out_trace("load file: %s\n", file_path);
	_err = cyaml_load_file(file_path, &config,
			&time_series_log_input_schema, (void **) pp_tsli, NULL);
	if (_err != CYAML_OK) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", cyaml_strerror(_err), __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// dump
	_ptsli = *pp_tsli;
	std_out_trace("title: %s\n", _ptsli->title);
	std_out_trace("variables:\n");
	for (i=0; i < _ptsli->time_series_log_variables_count; i++) {
		std_out_trace("%u\t%s\n", i, _ptsli->time_series_log_variables[i]);
	}
	std_out_trace("---\n");
	
	return EXIT_SUCCESS;
}

/// @brief 	Log the std out log data.
/// @param[in] file_path: std out log file path 
/// @param[out] pp_soli: std out log input data 
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure 
int load_std_out_log_input(char* file_path, std_out_log_input_t** pp_soli) {
	int i = 0, j = 0;
	cyaml_err_t _err;
	std_out_log_input_t* _psoli = NULL;

	if (file_path == NULL || pp_soli == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// load std out log input
	std_out_trace("load file: %s\n", file_path);
	_err = cyaml_load_file(file_path, &config,
			&std_out_log_input_schema, (void **) pp_soli, NULL);
	if (_err != CYAML_OK) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", cyaml_strerror(_err), __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// dump
	_psoli = *pp_soli;
	std_out_trace("title: %s\n", _psoli->title);
	std_out_trace("std out log:cyclic log variables:\n");
	for(i = 0; i < _psoli->cyclic_log_vairables_count; i++){
		std_out_trace("%u\t%s\n", i, _psoli->cyclic_log_vairables[i]);
	}

	std_out_trace("std out log:event log variables:\n");
	for (i=0; i < _psoli->event_logs_count; i++) {
		std_out_trace("%u\t%s\t%d\t%s\n", i, 
			_psoli->event_logs[i].variable, 
			_psoli->event_logs[i].opt_type, 
			_psoli->event_logs[i].threshold);

		std_out_trace("\trelation variables: %d\n", _psoli->event_logs[i].monitors_count);
		for(j=0; j < _psoli->event_logs[i].monitors_count; j++) {
			std_out_trace("\t%u\t%s\n", j, 
				_psoli->event_logs[i].monitors[j]);
		} 
	}	

	std_out_trace("---\n");
	
	return EXIT_SUCCESS;
}

/// @brief Initialize the user interface.
/// @param[in] adr_base: base address of the executable process
/// @param[in] sm: symbol map data
/// @param[in] setting: initialzation setting buffer
/// @param[in] count: number of initialization setting buffer	
/// @param[in,out] offset: offset of the initialized variable buffer
/// @note If sama variable is initialized, the value is overwritten and output warining message.
void init_param(ADDRESS_T adr_base, symbol_map_t* sm, init_setting_t *settings, uint32_t count, uint32_t *offset) {
	int i = 0, j = 0;
	int _ret = 0;
	int32_t _tmp_int = 0;
	double _tmp_dbl = 0;
	vec3_t _temp_vec = {0};
	quat_t _temp_quat  = {0};
	mat33_t _temp_mat33 = {0};
	symbol_type_e _type;
	ADDRESS_T _adr_map = 0;

	if (sm == NULL || settings == NULL || count == 0 || offset == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return;
	}	

	std_out_trace("init parameters and states\n");
	for(i=0; i < count; i++) {
		for(j=0; j < INIT_DATA_MAX; j++) {
			// printf("init_vars[%d]: %s\n", j, init_vars[j]);
			if (strlen(init_vars[j]) == 0){
				break;
			}
			if (strncmp(settings[i].variable, init_vars[j], LOG_NAME_SIZE) == 0) {
				std_out_trace("The specified variable (%s) is reinitialized. \n", settings[i].variable);
				break;
			}
		}

		_ret = get_symbol_address(settings[i].variable, sm, &_adr_map, &_type);
		if(EXIT_FAILURE == _ret) {
			std_out_trace("not found %s\n", settings[i].variable);
			continue;
		}
		switch (_type) {
		case SYMBOL_TYPE_INT:
			if (sscanf(settings[i].value, "%d", &_tmp_int) != 1) {
				std_out_trace("the value of %s is invalid. : %s", settings[i].variable, settings[i].value);  
			}
			memcpy((void*)(adr_base + _adr_map), &_tmp_int, sizeof(int32_t));
			break;
		case SYMBOL_TYPE_DBL:
			if (sscanf(settings[i].value, "%lf", &_tmp_dbl) != 1) {
				std_out_trace("the value of %s is invalid. : %s", settings[i].variable, settings[i].value);
			}  
			memcpy((void*)(adr_base + _adr_map), &_tmp_dbl, sizeof(double));
			break;
		case SYMBOL_TYPE_VECTOR:
			if (sscanf(settings[i].value, "%lf, %lf, %lf", &_temp_vec.x, &_temp_vec.y, &_temp_vec.z) != 3){
				std_out_trace("the value of %s is invalid. : %s", settings[i].variable, settings[i].value);
			}
			memcpy((void*)(adr_base + _adr_map), &_temp_vec, sizeof(vec3_t));
			break;
		case SYMBOL_TYPE_QUAT:
			if (sscanf(settings[i].value, "%lf,%lf,%lf,%lf", &_temp_quat.s, &_temp_quat.x, &_temp_quat.y, &_temp_quat.z) != 4){
				std_out_trace("the value of %s is invalid. : %s", settings[i].variable, settings[i].value);
			}
			memcpy((void*)(adr_base + _adr_map), &_temp_quat, sizeof(quat_t));
			break;
		case SYMBOL_TYPE_MATRIX:
			if (sscanf(settings[i].value, "%lf,%lf,%lf; %lf,%lf,%lf; %lf,%lf,%lf", 
					&_temp_mat33.a.x, &_temp_mat33.b.x, &_temp_mat33.c.x,
					&_temp_mat33.a.y, &_temp_mat33.b.y, &_temp_mat33.c.y,
					&_temp_mat33.a.z, &_temp_mat33.b.z, &_temp_mat33.c.z) != 9){
				std_out_trace("the value of %s is invalid. : %s", settings[i].variable, settings[i].value);
			}
			memcpy((void*)(adr_base + _adr_map), &_temp_mat33, sizeof(mat33_t));break;
			break;
		default:
			std_out_trace("unknown type %d for %s\n", _type, settings[i].variable);
			break;
		}

		strncpy(init_vars[*offset], settings[i].variable, LOG_NAME_SIZE);
		*offset = *offset + 1;
	}
	std_out_trace("---\n");	
}

/// @brief Create variable data.
/// @param[in] adr_base: base address of the executable process
/// @param[in] sm: symbol map data
/// @param[in] name: variable name
/// @param[out] var: variable data
void create_variable_data(ADDRESS_T adr_base, symbol_map_t* sm, const char* name, variable_data_t* var) {
	int _ret = 0;
	ADDRESS_T _adr_map = 0;
	symbol_type_e _type;

	if (sm == NULL || name == NULL || var == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return;
	}
	var->type = LOG_TYPE_UNKNOWN;
	var->adr = 0;

	_ret = get_symbol_address(name, sm, &_adr_map, &_type);
	if(EXIT_FAILURE == _ret) {
		std_out_trace("not found %s\n", name);
		return;
	}
	switch (_type) {
	case SYMBOL_TYPE_INT:
		var->type = LOG_TYPE_INT;
		break;
	case SYMBOL_TYPE_DBL:
		var->type = LOG_TYPE_DBL;
		break;
	default:
		std_out_trace("unknown type %d for %s\n", _type, name);
		break;
	}
	strcpy(var->name, name);
	var->adr = adr_base + _adr_map;
}

/// @brief Create time series log data.
/// @param[in] adr_base: base address of the executable process
/// @param[in] sm: symbol map data
/// @param[in] variables: variable name buffer
/// @param[in] variables_count: number of variable name buffer
/// @param[in,out] offset: offset of the time series log data buffer
void create_time_series_log_data(ADDRESS_T adr_base, symbol_map_t* sm, const char **variables, uint32_t variables_count, uint32_t *offset) {
	int i = 0, j = 0;
	int _ret = 0;
	time_series_log_data_t* _tsld_p = NULL;

	if (sm == NULL || variables == NULL || variables_count == 0 || offset == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return;
	}

	// create time series log data
	std_out_trace("create time series log data\n");
	j = *offset;
	for (i=0; i < variables_count; i++) {
		create_variable_data(adr_base, sm, variables[i], &tsld[j].v);

		if(0 < j) {
			tsld[j-1].next = (ADDRESS_T)&tsld[j];
		}
		j++;
		if(TIME_SERIES_LOG_DATA_MAX <= j) {
			std_out_trace("reached to TIME_SERIES_LOG_DATA_MAX\n");
			break;
		}
	}
	*offset = j;

	// dump
	std_out_trace("time series log data:\n");
	i = 0;
	_tsld_p = &tsld[0];
	while(i++, i < TIME_SERIES_LOG_DATA_MAX) {
		if (strlen(_tsld_p->v.name) > 0) {
			std_out_trace("%s\t%016lx\t%d\t%016lx\n", _tsld_p->v.name, _tsld_p->v.adr, _tsld_p->v.type, _tsld_p->next);
		}
		if(0 == _tsld_p->next) {
			break;
		}
		_tsld_p = (time_series_log_data_t*)_tsld_p->next;
	}
	std_out_trace("---\n");
}

/// @brief Create cyclic log data.
/// @param[in] adr_base: base address of the executable process 
/// @param[in] sm: symbol map data 
/// @param[in] variables: variable name buffer 
/// @param[in] variables_count: number of variable name buffer 
/// @param[in,out] offset: offset of the cyclic log data buffer 
void create_cyclic_log_data(ADDRESS_T adr_base, symbol_map_t* sm, const char **variables, uint32_t variables_count, uint32_t *offset) {
	int i = 0, j = 0;
	int _ret = 0;
	cyclic_log_data_t* _cold = NULL;

	if (sm == NULL || variables == NULL || variables_count == 0 || offset == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return;
	}

	// create cyclic log data
	std_out_trace("create std out log data\n");
	j = *offset;
	for(i = 0; i < variables_count; i++){
		create_variable_data(adr_base, sm, variables[i], &cold[j].v);

		if(0 < j) {
			cold[j-1].next = (ADDRESS_T)&cold[j];
		}
		j++;
		if(CYCLIC_LOG_DATA_MAX <= j) {
			std_out_trace("reached to CYCLIC_LOG_DATA_MAX\n");
			break;
		}
	}
	*offset = j;

	// dump
	std_out_trace("std out log data:\n");
	i = 0;
	_cold = &cold[0];
	while(i++, i < TIME_SERIES_LOG_DATA_MAX) {
		if (strlen(_cold->v.name) > 0) {
			std_out_trace("%s\t%016lx\t%d\t%016lx\n", _cold->v.name, _cold->v.adr, _cold->v.type, _cold->next);
		}
		if(0 == _cold->next) {
			break;
		}
		_cold = (cyclic_log_data_t*)_cold->next;
	}
	std_out_trace("---\n");
}

/// @brief Create event log data.
/// @param[in] adr_base: base address of the executable process 
/// @param[in] sm: symbol map data 
/// @param[in] event_logs: event log data buffer
/// @param[in] event_logs_count: number of event log data buffer
/// @param[in,out] offset: offset of the event log data buffer 
void create_event_log_data(ADDRESS_T adr_base, symbol_map_t* sm, event_log_t * event_logs, uint32_t event_logs_count, uint32_t *offset) {
	int i = 0, j = 0, k = 0, l = 0;

	int _ret = 0;
	event_log_data_t* _evld = NULL;
	event_monitor_data_t * _evmd = NULL;
	int32_t _tmp_int = 0;
	double _tmp_dbl = 0;
	
	if (sm == NULL || event_logs == NULL || event_logs_count == 0 || offset == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return;
	}

	// create event log data
	std_out_trace("create event log data\n");
	j = *offset;
	for(i = 0; i < event_logs_count; i++){
		create_variable_data(adr_base, sm, event_logs[i].variable, &evld[j].v);

		l = 0;
		for(k = 0; k < event_logs[i].monitors_count; k++){
			create_variable_data(adr_base, sm, event_logs[i].monitors[k], &evld[j].evmd[l].v);
			if(0 < l) {
				evld[j].evmd[l-1].next = (ADDRESS_T)&evld[j].evmd[l];
			}
			l++;
			if(EVENT_MONITOR_DATA_MAX <= l) {
				std_out_trace("reached to EVENT_MONITOR_DATA_MAX\n");
				break;
			}		
		}

		evld[j].opt_type = event_logs[i].opt_type;

		if (evld[j].opt_type == OPT_TYPE_LTE ||
		    evld[j].opt_type == OPT_TYPE_LT  ||
			evld[j].opt_type == OPT_TYPE_GTE ||
			evld[j].opt_type == OPT_TYPE_GT  ||
			evld[j].opt_type == OPT_TYPE_EQ  ||
			evld[j].opt_type == OPT_TYPE_NEQ) {
			if (event_logs[i].threshold != NULL) {
				switch (evld[j].v.type) {
				case SYMBOL_TYPE_INT:
					if (sscanf(event_logs[i].threshold, "%d", &_tmp_int) != 1) {
						std_out_trace("threshold format of %s is invalid. : %s", event_logs[i].variable, event_logs[i].threshold);  
					}
					memcpy(&evld[j].thr.i_value, &_tmp_int, sizeof(int32_t));
					break;
				case SYMBOL_TYPE_DBL:
					if (sscanf(event_logs[i].threshold, "%lf", &_tmp_dbl) != 1) {
						std_out_trace("threshold format of %s is invalid. : %s", event_logs[i].variable, event_logs[i].threshold);
					}  
					memcpy(&evld[j].thr.d_value, &_tmp_dbl, sizeof(double));
					break;
				default:
					break;
				}
			}
		}

		evld[j].eval_type = EVAL_NONE;

		if(0 < j) {
			evld[j-1].next = (ADDRESS_T)&evld[j];
		}
		j++;
		if(EVENT_LOG_DATA_MAX <= j) {
			std_out_trace("reached to EVENT_LOG_DATA_MAX\n");
			break;
		}
	}
	*offset = j;

	// dump
	std_out_trace("event log data:\n");
	i = 0;
	k = 0;
	_evld = &evld[0];
	while(i++, i < EVENT_LOG_DATA_MAX) {
		if (strlen(_evld->v.name) > 0) {
			std_out_trace("%s\t%016lx\t%d\t%d\t%f\t%016lx\n", _evld->v.name, _evld->v.adr, _evld->v.type, _evld->opt_type, _evld->thr.d_value, _evld->next);
			_evmd = &_evld->evmd[0];
			if (strlen(_evmd->v.name) > 0) {
				while(k++, k < EVENT_MONITOR_DATA_MAX) {
					std_out_trace("\t%s\t%016lx\t%d\t%016lx\n", _evmd->v.name, _evmd->v.adr, _evmd->v.type, _evmd->next);
					if(0 == _evmd->next) {
						break;
					}
					_evmd = (event_monitor_data_t*)_evmd->next;			
				}
			}
		}
		if(0 == _evld->next) {
			break;
		}
		_evld = (event_log_data_t*)_evld->next;
	}
	std_out_trace("---\n");
}

/// @brief Write time series log header.
void write_time_series_log_header() {
	int i = 0;
	time_series_log_data_t* _tsld_p = NULL;

	if (uip.time_series_log_cycle == 0) return;

	_tsld_p = &tsld[0];
	i = 0;
	fprintf(tslfp,"sim_time");
	while(i++ < TIME_SERIES_LOG_DATA_MAX) {
		if (strlen(_tsld_p->v.name) > 0) {
			fprintf(tslfp, "\t%s", _tsld_p->v.name);
		}
		if(0 == _tsld_p->next) {
			break;
		}
		_tsld_p = (time_series_log_data_t*)_tsld_p->next;
	}
	fprintf(tslfp, "\n");
}

/// @brief Write time series log frame.
/// @param[in] cycle: cycle count
/// @param[in] t: simulation time
void write_time_series_log_frame(int cycle, double t) {
	int i = 0;
	time_series_log_data_t* _tsld_p = NULL;

	if (uip.time_series_log_cycle == 0) return;

	if (0 == ((cycle+1)%uip.time_series_log_cycle)) {
		fprintf(tslfp, "%f", t);
		_tsld_p = &tsld[0];
		i = 0;
		while(i++ < TIME_SERIES_LOG_DATA_MAX) {
			if (strlen(_tsld_p->v.name) > 0) {
				switch (_tsld_p->v.type) {
				case LOG_TYPE_INT:
					fprintf(tslfp, "\t%d", *(int*)_tsld_p->v.adr);
					break;
				case LOG_TYPE_DBL:
					fprintf(tslfp, "\t%.12f", *(double*)_tsld_p->v.adr);
					break;
				default:
					// this case is excluded when time series log data was created
					break;
				}
			}
			if(0 == _tsld_p->next) {
				break;
			}
			_tsld_p = (time_series_log_data_t*)_tsld_p->next;
		}
		fprintf(tslfp, "\n");
		fflush(tslfp);
	}
}

/// @brief Write cyclic log header.
void write_cyclic_log_header() {
	int i = 0;
	cyclic_log_data_t* _cold = NULL;

	if (uip.std_out_log_cycle == 0)  return;

	std_out_trace("sim_time:");
	_cold = &cold[0];
	i = 0;
	while(i++ < CYCLIC_LOG_DATA_MAX) {
		if (strlen(_cold->v.name) > 0) {
			std_out_trace("\t%s", _cold->v.name);
		}
		if (0 == _cold->next) {
			break;
		}
		_cold = (cyclic_log_data_t*)_cold->next;
	}
	std_out_trace("\n");	
}

/// @brief Write cyclic log frame.
/// @param[in] cycle: cycle count
/// @param[in] t: simulation time
void write_cyclic_log_frame(int cycle, double t) {
	int i = 0;
	cyclic_log_data_t* _cold = NULL;

	if (uip.std_out_log_cycle == 0)  return;

	if (0 == ((cycle+1)%uip.std_out_log_cycle)) {
		std_out_trace("%f:", t);
		_cold = &cold[0];
		i = 0;

		while(i++ < CYCLIC_LOG_DATA_MAX) {
			if (strlen(_cold->v.name) > 0) {
				switch (_cold->v.type) {
				case LOG_TYPE_INT:
				std_out_trace("\t%d", *(int*)_cold->v.adr);
					break;
				case LOG_TYPE_DBL:
				std_out_trace("\t%.12f", *(double*)_cold->v.adr);
					break;
				default:
					// this case is excluded when std out log data was created
					break;
				}
			}
			if(0 == _cold->next) {
				break;
			}
			_cold = (cyclic_log_data_t*)_cold->next;
		}
		std_out_trace("\n");
	}
}

/// @brief Write event log frame.
/// @param[in] _evld: event log data
void write_event_monitor(event_log_data_t* _evld) {
	int i = 0;
	event_monitor_data_t* _evmd = &_evld->evmd[0];

	if (uip.std_out_log_cycle == 0)  return;
	
	if (_evld == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return;
	}

	while (i++ < EVENT_MONITOR_DATA_MAX) {
		if (strlen(_evmd->v.name) > 0) {
			if (i <= 1) {
				std_out_trace("  monitor:\n");
			}
			switch (_evmd->v.type) {
			case LOG_TYPE_INT:
				std_out_trace("   %s: %d\n", _evmd->v.name, *(int*)_evmd->v.adr);
				break;
			case LOG_TYPE_DBL:
				std_out_trace("   %s: %.12f\n",  _evmd->v.name, *(double*)_evmd->v.adr);
				break;
			default:
				// this case is excluded when std out log data was created
				break;
			}
		}
		if(0 == _evmd->next) {
			break;
		}
		_evmd = (event_monitor_data_t*)_evmd->next;
	}
}

/// @brief Write event log. (int version)
/// @param[in] opt_type: option type 
/// @param[in] t: simulation time 
/// @param[in] name: variable name 
/// @param[in] cur: current value 
/// @param[in] prv: previous value 
void write_event_int(option_type_e opt_type, double t, char* name, int cur, int prv) {

	if (uip.std_out_log_cycle == 0)  return;

	if (name == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return;
	}

	switch(opt_type) {
		case OPT_TYPE_CHG:
			std_out_trace("event CHG %f: %s %d -> %d\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_LTE:
			std_out_trace("event LTE %f: %s %d -> %d\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_LT:
			std_out_trace("event LT  %f: %s %d -> %d\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_GTE:
			std_out_trace("event GTE %f: %s %d -> %d\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_GT:
			std_out_trace("event GT  %f: %s %d -> %d\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_EQ:
			std_out_trace("event EQ  %f: %s %d -> %d\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_NEQ:
			std_out_trace("event NEQ %f: %s %d -> %d\n", t, (char*)name, prv, cur);
			break;
		default:
			break;
	}
}

/// @brief Write event log. (double version)
/// @param[in] opt_type: option type 
/// @param[in] t: simulation time 
/// @param[in] name: variable name 
/// @param[in] cur: current value 
/// @param[in] prv: previous value 
void write_event_double(option_type_e opt_type, double t, char* name, double cur, double prv) {

	if (uip.std_out_log_cycle == 0)  return;

	if (name == NULL) {
		stderr_out_trace("ERROR: %s (%s:%d)\n", "The specified parameter is invalid.", __FILE__, __LINE__);
		return;
	}

	switch(opt_type) {
		case OPT_TYPE_CHG:
			std_out_trace("event CHG %f: %s %.12f -> %.12f\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_LTE:
			std_out_trace("event LTE %f: %s %.12f -> %.12f\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_LT:
			std_out_trace("event LT  %f: %s %.12f -> %.12f\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_GTE:
			std_out_trace("event GTE %f: %s %.12f -> %.12f\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_GT:
			std_out_trace("event GT  %f: %s %.12f -> %.12f\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_EQ:
			std_out_trace("event EQ  %f: %s %.12f -> %.12f\n", t, (char*)name, prv, cur);
			break;
		case OPT_TYPE_NEQ:
			std_out_trace("event NEQ %f: %s %.12f -> %.12f\n", t, (char*)name, prv, cur);
			break;
		default:
			break;
	}
}

/// @brief Check the event condition.(int version)
/// @param[in] opt_type: option type
/// @param[in] src: source value
/// @param[in] dest: destination value
/// @param[in] thr: threshold value
/// @return EVAL_TRUE: true, EVAL_FALSE: false, EVAL_NONE: none
eval_type_e check_condition_int(option_type_e opt_type, int src, int dest, int thr) {
	eval_type_e _ret = EVAL_NONE;

	switch(opt_type) {
		case OPT_TYPE_CHG:
			_ret = CHG(src, dest) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_LTE:
			_ret = LTE(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_LT:
			_ret = LT(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_GTE:
			_ret = GTE(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_GT:
			_ret = GT(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_EQ:
			_ret = EQ(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_NEQ:
			_ret = NEQ(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		default:
			break;
	}
	return _ret;
}

/// @brief Check the event condition.(double version)
/// @param[in] opt_type: option type
/// @param[in] src: source value
/// @param[in] dest: destination value
/// @param[in] thr: threshold value
/// @return EVAL_TRUE: true, EVAL_FALSE: false, EVAL_NONE: none
eval_type_e check_condition_double(option_type_e opt_type, double src, double dest, double thr) {
	eval_type_e _ret = EVAL_NONE;

	switch(opt_type) {
		case OPT_TYPE_CHG:
			_ret = CHG(src, dest) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_LTE:
			_ret = LTE(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_LT:
			_ret = LT(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_GTE:
			_ret = GTE(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_GT:
			_ret = GT(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_EQ:
			_ret = EQ(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		case OPT_TYPE_NEQ:
			_ret = NEQ(src, thr) ? EVAL_TRUE : EVAL_FALSE;
			break;
		default:
			break;
	}
	return _ret;
}

/// @brief Check the event.
/// @param[in] t: simulation time
void check_event(double t) {
	int i = 0;
	int _ret = 0;
	event_log_data_t* _evld = NULL;
	_evld = &evld[0];

	while(i++ < EVENT_LOG_DATA_MAX) {
		double thr_d, cur_d, prv_d;
		int32_t thr_n, cur_n, prv_n;

		if (strlen(_evld->v.name) > 0) {
			switch(_evld->v.type) {
				case LOG_TYPE_INT:
					thr_n = _evld->thr.i_value;
					cur_n = *(int32_t*)_evld->v.adr;
					prv_n = _evld->prv.i_value;
					if (check_condition_int(_evld->opt_type, cur_n, prv_n, thr_n) == EVAL_TRUE) {
						if (_evld->eval_type == EVAL_FALSE) {
							write_event_int(_evld->opt_type, t, _evld->v.name, cur_n, prv_n);
							write_event_monitor(_evld);
						}
						_evld->eval_type = EVAL_TRUE; 
					}
					else {
						_evld->eval_type = EVAL_FALSE; 
					}
					_evld->prv.i_value = cur_n;
					break;
				case LOG_TYPE_DBL:
					thr_d = _evld->thr.d_value;
					cur_d = *(double*)_evld->v.adr;
					prv_d = _evld->prv.d_value;
					if (check_condition_double(_evld->opt_type, cur_d, prv_d, thr_d) == EVAL_TRUE) {
						if (_evld->eval_type == EVAL_FALSE) {
							write_event_double(_evld->opt_type, t, _evld->v.name, cur_d, prv_d);
							write_event_monitor(_evld);
						}
						_evld->eval_type = EVAL_TRUE; 
					}
					else {
						_evld->eval_type = EVAL_FALSE; 
					}
					_evld->prv.d_value = cur_d;
					break;
				default:
					break;
			}
		}
		if(0 == _evld->next) {
			break;
		}
		_evld = (event_log_data_t*)_evld->next;
	}
}

/// @brief Initialize userif
/// @param [in] t: simulation time 
/// @param [in] test_case_file: test case file path
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
/// @note The param (t) is not used.
int userif_init(double t, char* test_case_file) {
	int _ret;
	int i = 0;
	cyaml_err_t _err;
	symbol_map_t* _psm = NULL;
	test_case_t* _ptc = NULL;
	init_input_t* _pin_arr[INPUT_FILE_NAME_SIZE] = {0};
	time_series_log_input_t* _ptsli_arr[INPUT_FILE_NAME_SIZE] = {0};
	std_out_log_input_t* _psoli_arr[INPUT_FILE_NAME_SIZE] = {0};
	ADDRESS_T _adr_base = 0;
	char exe_dir[PATH_MAX-NAME_MAX] = {0};
	char input_file_path[PATH_MAX] = {0};
	char output_file_path[PATH_MAX] = {0};
	uint32_t _tsld_offset = 0, _cld_offset = 0, _evld_offset = 0, _init_vars_offset = 0;

	get_exe_dir(exe_dir, sizeof(exe_dir));

	// open std out log file
	sprintf(std_log_out_temp_file_path, "%s/%s", exe_dir, STD_OUT_LOG_TEMP_FILE_NAME);
	_ret = std_trace_init(std_log_out_temp_file_path);
	if (_ret != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	// start message
	std_out_trace("avisyslator gnc start\n");
	std_out_trace("case file : %s\n", test_case_file);
	std_out_trace("---\n");

	// initialize start message
	std_out_trace("init start\n");
	std_out_trace("---\n");

	// load symbol map
	sprintf(input_file_path, "%s/%s", exe_dir, SYMBOL_MAP_FILE_NAME);
	_ret = load_symbol_map(input_file_path, &_adr_base, &_psm);
	if (_ret != EXIT_SUCCESS) {
		return _ret;
	}

	// load test case yaml
	sprintf(input_file_path, "%s/%s", exe_dir, test_case_file);
	_ret = load_test_case(input_file_path, &_ptc);
	if (_ret != EXIT_SUCCESS) {
		return _ret;
	}


	// load init input yaml
	if (_ptc->init != NULL) {
		if(EVENT_LOG_DATA_MAX < _ptc->init->input_files_count) {
			std_out_trace("%s input files count is reached to INPUT_FILE_NAME_SIZE\n", "init");
		}
		for (i = 0; i < _ptc->init->input_files_count; i++) {
			if (INPUT_FILE_NAME_SIZE <= i) break;
			sprintf(input_file_path, "%s/%s", exe_dir, _ptc->init->input_files[i]);
			_ret = load_init_input(input_file_path, &_pin_arr[i]);
			if (_ret != EXIT_SUCCESS) {
				return _ret;
			}
		}
	}

	// load time series input yaml
	if (_ptc->time_series_log != NULL) {
		if(EVENT_LOG_DATA_MAX < _ptc->time_series_log->input_files_count) {
			std_out_trace("%s input files count is reached to INPUT_FILE_NAME_SIZE\n", "time_series_log");
		}
		for (i = 0; i < _ptc->time_series_log->input_files_count; i++) {
			if (INPUT_FILE_NAME_SIZE <= i) break;
			sprintf(input_file_path, "%s/%s", exe_dir, _ptc->time_series_log->input_files[i]);
			_ret = load_time_series_log_input(input_file_path, &_ptsli_arr[i]);
			if (_ret != EXIT_SUCCESS) {
				return _ret;
			}
		}
	}

	// load std out log input yaml
	if (_ptc->std_out_log != NULL) {
		if(EVENT_LOG_DATA_MAX < _ptc->std_out_log->input_files_count) {
			std_out_trace("%s input files count is reached to INPUT_FILE_NAME_SIZE\n", "std_out_log");
		}
		for (i = 0; i < _ptc->std_out_log->input_files_count; i++) {
			if (INPUT_FILE_NAME_SIZE <= i) break;
			sprintf(input_file_path, "%s/%s", exe_dir, _ptc->std_out_log->input_files[i]);
			_ret = load_std_out_log_input(input_file_path, &_psoli_arr[i]);
			if (_ret != EXIT_SUCCESS) {
				return _ret;
			}
		}
	}

	// init parameter and states
	if (_ptc->init != NULL) {
		for (i = 0; i < _ptc->init->input_files_count; i++) {
			if (INPUT_FILE_NAME_SIZE <= i) break;
			init_param(_adr_base, _psm, _pin_arr[i]->init_setting, _pin_arr[i]->init_setting_count, &_init_vars_offset);
		}
		init_param(_adr_base, _psm, _ptc->init->init_settings, _ptc->init->init_settings_count, &_init_vars_offset);
	}

	// create time series log data
	if (_ptc->time_series_log != NULL) {
		for (i = 0; i < _ptc->time_series_log->input_files_count; i++) {
			if (INPUT_FILE_NAME_SIZE <= i) break;
			create_time_series_log_data(_adr_base, _psm, _ptsli_arr[i]->time_series_log_variables, _ptsli_arr[i]->time_series_log_variables_count, &_tsld_offset);
		}
		create_time_series_log_data(_adr_base, _psm, _ptc->time_series_log->time_series_log_variables, _ptc->time_series_log->time_series_log_variables_count, &_tsld_offset);
	}

	// create cyclic log data and event log data
	if (_ptc->std_out_log != NULL) {
		for (i = 0; i < _ptc->std_out_log->input_files_count; i++) {
			if (INPUT_FILE_NAME_SIZE <= i) break;
			create_cyclic_log_data(_adr_base, _psm, _psoli_arr[i]->cyclic_log_vairables, _psoli_arr[i]->cyclic_log_vairables_count, &_cld_offset);
			create_event_log_data(_adr_base, _psm, _psoli_arr[i]->event_logs, _psoli_arr[i]->event_logs_count, &_evld_offset);
		}
		create_cyclic_log_data(_adr_base, _psm, _ptc->std_out_log->cyclic_log_vairables, _ptc->std_out_log->cyclic_log_vairables_count, &_cld_offset);
		create_event_log_data(_adr_base, _psm, _ptc->std_out_log->event_logs, _ptc->std_out_log->event_logs_count, &_evld_offset);
	}

	if (_ptc->std_out_log != NULL) {
		// create log file path
		sprintf(std_out_log_file_path, "%s/%s", exe_dir, _ptc->std_out_log->output_file);
	}

	if (_ptc->time_series_log != NULL) {
		// open time series log file
		sprintf(output_file_path, "%s/%s", exe_dir, _ptc->time_series_log->output_file);
		tslfp = fopen(output_file_path, "w");
		if (tslfp == NULL) {
			printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
			return EXIT_FAILURE;
		}
	}	

	/* free the data */
	cyaml_free(&config, &symbol_map_schema, _psm, 0);
	cyaml_free(&config, &test_case_schema, _ptc, 0);
	if (_ptc->init != NULL) {
		for (i = 0; i < _ptc->init->input_files_count; i++) {
			cyaml_free(&config, &init_input_schema, _pin_arr[i], 0);
		}
	}
	if (_ptc->time_series_log != NULL) {
		for (i = 0; i < _ptc->time_series_log->input_files_count; i++) {
			cyaml_free(&config, &time_series_log_input_schema, _ptsli_arr[i], 0);
		}
	}
	if (_ptc->std_out_log != NULL) {
		for (i = 0; i < _ptc->std_out_log->input_files_count; i++) {
			cyaml_free(&config, &std_out_log_input_schema, _psoli_arr[i], 0);
		}
	}
	// initialize finish message
	std_out_trace("init end\n");
	std_out_trace("---\n");

	return EXIT_SUCCESS;
}

/// @brief Initialize main loop
/// @param[in] t: simulation time
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
int userif_main_loop_start(double t) {
	// message
	std_out_trace("simulation loop start\n");
	
	// time series log header
	write_time_series_log_header();

	// std out log header
	write_cyclic_log_header();

	userif_main((-1), t);
	return EXIT_SUCCESS;
}

/// @brief Terminate main loop
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
int userif_main_loop_end() {
	std_out_trace("simulation loop end\n");
	std_out_trace("---\n");
	return EXIT_SUCCESS;
}

/// @brief Execute main loop
/// @param[in] cycle: cycle count
/// @param[in] t: simulation time
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
int userif_main(int cycle, double t) {
		// time serise log
	write_time_series_log_frame(cycle, t);

	// std out log
	write_cyclic_log_frame(cycle, t);

	// check event
	check_event(t);

	return EXIT_SUCCESS;
}

/// @brief Terminate userif
/// @return EXIT_SUCCESS: Success, EXIT_FAILURE: Failure
/// @note If the process exits without calling this function, the log file will not be saved.
int userif_finish(void) {
	if (tslfp != NULL) {
		// end message
		std_out_trace("avisyslator gnc end\n");

		// close files
		fclose(tslfp);
		std_trace_term();
		rename(std_log_out_temp_file_path, std_out_log_file_path);
		tslfp = NULL;
	}	
	return EXIT_SUCCESS;
}
