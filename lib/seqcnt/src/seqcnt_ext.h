/// @file seqcnt_ext.h
/// @brief functions and structures of seqcnt externals

#ifndef __SEQCNT_EXT_H__
#define __SEQCNT_EXT_H__

#include <stdint.h>

#define SEQCNT_STOP (0)
#define SEQCNT_EXEC (1)

typedef struct {
	int32_t mode;
	int32_t id;
	int32_t step;
	int32_t error;
} seqcnt_state_t;


#endif
