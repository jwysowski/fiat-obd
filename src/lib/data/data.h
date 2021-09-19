#ifndef PL_H
#define PL_H

#define UNITS_SIZE              31
#define INFO_SIZE               31
#define ERRORS_SIZE             3
#define ACTIVE_TESTS_SIZE       7
#define ADJUSTMENTS_SIZE        5

enum engine_info_key {
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

enum active_tests_key {
	FUEL_PUMP,
	INJECTOR,
	COIL1,
	COIL2,
	EVAP,
	REV_METER,
	AC_RELAY,
	GENERIC_RELAY
};

enum adjustments_key {
	TOGGLE_TRIM_AC,
	TOGGLE_STEP_AC,
	TRIM_RESET,
	STEP_RESET,
	TRIM_SET,
	STEP_SET
};

enum error_key {
	NO_SYNC_ERR,
	START_DIS_ERR,
	UNI_CODE_ERR
};

struct adjustments_map {
	enum adjustments_key	key;
	char			*description;
	uint8_t			status;
	uint8_t			status_mask;
	int			pre_set_number_of_bytes;
	int			post_set_number_of_bytes;
	char			*type;
};


struct decode_map {
	enum engine_info_key	key;
	double			(*function)(uint8_t *);
};


struct units_map {
	enum engine_info_key	key;
	char			*value;
};

struct engine_map {
	enum engine_info_key	key;
	char			*description;
	int			number_of_bytes;
};

struct active_tests_map {
	enum active_tests_key	key;
	char			*description;
	int			is_engine_on;
	uint8_t			timeout;
};


struct errors_map {
	enum error_key	key;
	char		*description;
};

extern struct adjustments_map adjustments[ADJUSTMENTS_SIZE];
extern struct active_tests_map tests[ACTIVE_TESTS_SIZE];
extern struct decode_map functions[INFO_SIZE];
extern struct units_map units[UNITS_SIZE];
extern struct engine_map engine_info[INFO_SIZE];
extern struct errors_map errors[ERRORS_SIZE];

#endif
