#ifndef PL_H
#define PL_H

#include "../data_structs.h"

#define UNITS_SIZE              31
#define INFO_SIZE               31
#define ENGINE_ERRORS_SIZE      23
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

enum engine_error_key {
	ERR_TPS,
	ERR_MAP,
	ERR_LAMBDA,
	ERR_ECT,
	ERR_IAT,
	ERR_BATT_V,
	ERR_IDLE_REG,
	ERR_I8,
	ERR_INJ,
	ERR_COIL1,
	ERR_COIL2,
	ERR_IAV,
	ERR_EVAP,
	ERR_AIR_CO,
	ERR_FUEL_PUMP,
	ERR_GEN_REL,
	ERR_AC_PARAM,
	ERR_RAM,
	ERR_ROM,
	ERR_EEPROM,
	ERR_CPU,
	ERR_RPM_SENS,
	ERR_F7,
	ERR_F8
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


struct engine_info_decode {
	enum engine_info_key	key;
	double			(*info_decode)(uint8_t *);
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
	enum engine_error_key	key;
	char			*description;

	uint8_t			ra_base;
	uint8_t			rv_base;
	uint8_t			rs_base;
	uint8_t			r_base;
	uint8_t			ra_ext;
	uint8_t			rv_ext;
	uint8_t			rs_ext;
	uint8_t			rb_ext;
	char			*h_ext;
	char			*l_ext;

	struct error_state	*(*err_decode)(struct error_element *, uint8_t *);
};

extern struct adjustments_map adjustments[ADJUSTMENTS_SIZE];
extern struct active_tests_map tests[ACTIVE_TESTS_SIZE];
extern struct engine_info_decode info_decode_functions[INFO_SIZE];
extern struct units_map units[UNITS_SIZE];
extern struct engine_map engine_info[INFO_SIZE];
extern struct errors_map engine_errors[ENGINE_ERRORS_SIZE];

#endif
