#ifndef PL_H
#define PL_H

#define UNITS_SIZE      5
#define INFO_SIZE       5
#define ERRORS_SIZE     5

enum key_info {
	PERIODE,
	T_INJ_AP,
	AVANCE,
	MP2_MP8,
	MT_AIR_L,
	MT_EAU_L,
	ANG_PAP0,
	V_BATT,
	K_O2,
	ALFAR,
	INTEGR,
	PROP,
	TRIMRAM,
	ALFAU,
	ALFAUC,
	CONS_REG,
	OFNNTR,
	DELREG,
	ALFAFTR,
	ADOFFSET,
	ADOFFPURG,
	ADOFFPL,
	ADGAIN,
	STA_ENG_RUN,
	STA_SENS_OK,
	STA_THR_MM,
	STA_CL_LOOP,
	STA_AIR_CO_ON,
	STA_MIX_ACOK,
	STA_STEP_ACOK,
	STA_P_LATCH_OK
};

enum key_error {
	NO_SYNC_ERR,
	START_DIS_ERR,
	UNI_CODE_ERR
};

struct decode_info_map {
	enum key_info	key;
	void		(*function)();
};


struct units_info_map {
	enum key_info	key;
	char		*value;
};

struct engine_info_map {
	enum key_info	key;
	char		*value;
	int		number_of_bytes;
	void		(*decode)();
};

struct error_map {
	enum key_error	key;
	char		*value;
};
extern struct decode_info_map functions[INFO_SIZE];
extern struct units_info_map units[UNITS_SIZE];
extern struct engine_info_map engine_info[INFO_SIZE];
extern struct error_map errors[ERRORS_SIZE];

#endif
