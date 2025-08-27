#define FSEQ_STOP (0x00)
#define FSEQ_WAIT_MEIG (0x01)
#define FSEQ_WAIT_LIFTOFF (0x02)
#define FSEQ_WAIT_MEPO (0x03)
#define FSEQ_WAIT_MECO (0x04)
#define FSEQ_WAIT_MSEP (0x05)
#define FSEQ_WAIT_VLV_CLOSE (0x10)

typedef struct {
	int state;
} fcu_if_set_t;

fcu_if_set_t fcu_if_set = {FSEQ_STOP};

#define FMODE_OFF (0x10)
#define FMODE_ON (0x11)
#define GSE_CMD_NONE (0x20)
#define GSE_CMD_STOP (0x21)

typedef struct {
	int flight_mode;
	int stop_from_gse;
} fcu_if_get_t;

fcu_if_get_t fcu_if_get = {FMODE_OFF, GSE_CMD_NONE};
