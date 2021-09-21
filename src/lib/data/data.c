#include <stdio.h>
#include <stdint.h>
#include "data.h"

static const char *engine_error_find(enum engine_error_key key);
static const char *engine_info_find(enum engine_info_key key);

/*Engine info decoding functions*/
static double periode(uint8_t *buffer);
static double t_inj_ap(uint8_t *buffer);
static double mp2_mp8(uint8_t *buffer);
static double avance(uint8_t *buffer);
static double mt_air_l(uint8_t *buffer);
static double mt_eau_l(uint8_t *buffer);
static double ang_pap0(uint8_t *buffer);
static double v_batt(uint8_t *buffer);
static double k_02(uint8_t *buffer);
static double alfar(uint8_t *buffer);
static double integr(uint8_t *buffer);
static double prop(uint8_t *buffer);
static double trimram(uint8_t *buffer);
static double alfau(uint8_t *buffer);
static double alfauc(uint8_t *buffer);
static double cons_reg(uint8_t *buffer);
static double ofnntr(uint8_t *buffer);
static double delreg(uint8_t *buffer);
static double alfaftr(uint8_t *buffer);
static double adoffset(uint8_t *buffer);
static double adoffpurg(uint8_t *buffer);
static double adoffpl(uint8_t *buffer);
static double adgain(uint8_t *buffer);
static double sta_eng_run(uint8_t *buffer);
static double sta_sens_ok(uint8_t *buffer);
static double sta_thr_mm(uint8_t *buffer);
static double sta_cl_loop(uint8_t *buffer);
static double sta_air_co_on(uint8_t *buffer);
static double sta_mix_acok(uint8_t *buffer);
static double sta_step_acok(uint8_t *buffer);
static double sta_p_latch_ok(uint8_t *buffer);

/*Engine errors decoding functions*/
static struct error_state *err16f(struct error_element *state, uint8_t *buffer);
static struct error_state *err_code(struct error_element *state, uint8_t *buffer);

struct immo_errors_map immo_errors[] = {
	{ ERR_NO_SYNC,	 "Centralka CODE niezaprogramowana",		0x71, 0, 0x72, 0, 0, 0, 0, 0, "", "", &err_code },
	{ ERR_START_DIS, "Brak zgody na uruchomienie silnika",		0x71, 0, 0x72, 1, 0, 0, 0, 0, "", "", &err_code },
	{ ERR_UNI_CODE,	 "Centralka otrzymała 'kod uniwersalny'",	0x71, 0, 0x72, 2, 0, 0, 0, 0, "", "", &err_code },
	{ ERR_C4,	 "Nieznany błąd CODE (4",			0x71, 0, 0x72, 3, 0, 0, 0, 0, "", "", &err_code },
	{ ERR_BACKDOOR,	 "Aktyowowano obejście",			0x71, 0, 0x72, 4, 0, 0, 0, 0, "", "", &err_code },
	{ ERR_KEY_CODE,	 "Błędny kod klucza",				0x71, 0, 0x72, 5, 0, 0, 0, 0, "", "", &err_code },
	{ ERR_UNR_CODE,	 "Nieprawidłowy bądź nierozpoznany kod klucza", 0x71, 0, 0x72, 6, 0, 0, 0, 0, "", "", &err_code },
	{ ERR_LINK_DOWN, "Nie odebrano kodu lub brak połączenia",	0x71, 0, 0x72, 7, 0, 0, 0, 0, "", "", &err_code }
};


struct engine_errors_map engine_errors[] = {
	{ ERR_TPS,	 "Czujnik położenia przepustnicy", 0x10, 0, 0x14, 0, 0x2B, 0, 0x2E, 0, "Zwarcie do GND",	     "Zwarcie do Vcc",	       &err16f },
	{ ERR_MAP,	 "Czujnik MAP",			   0x10, 0, 0x14, 1, 0x2B, 0, 0x2E, 1, "Zwarcie do Vcc",	     "Zwarcie do GND",	       &err16f },
	{ ERR_LAMBDA,	 "Sonda lambda",		   0x10, 0, 0x14, 2, 0x2B, 0, 0x2E, 2, "",			     "",		       &err16f },
	{ ERR_ECT,	 "Czujnik temperatury wody",	   0x10, 0, 0x14, 3, 0x2B, 0, 0x2E, 3, "Zwarcie do GND",	     "Zwarcie do Vcc",	       &err16f },
	{ ERR_IAT,	 "Czujnik temperatury powietrza",  0x10, 0, 0x14, 4, 0x2B, 0, 0x2E, 4, "Zwarcie do GND",	     "Zwarcie do Vcc",	       &err16f },
	{ ERR_BATT_V,	 "Napięcie akumulatora",	   0x10, 0, 0x14, 5, 0x2B, 0, 0x2E, 5, "Ponad MAX",		     "Poniżej MIN",	       &err16f },
	{ ERR_IDLE_REG,	 "Regulacja biegu jałowego",	   0x10, 0, 0x14, 6, 0x2B, 0, 0x2E, 6, "",			     "",		       &err16f },
	{ ERR_I8,	 "Nieznay błąd INPUT (8)",	   0x10, 0, 0x14, 7, 0x2B, 0, 0x2E, 7, "",			     "",		       &err16f },
	{ ERR_INJ,	 "Wtryskiwacz",			   0x11, 0, 0x15, 0, 0x2C, 0, 0x2F, 0, "Zwarcie lub zab. termiczne", "Przerwa",		       &err16f },
	{ ERR_COIL1,	 "Cewka zapłonowa 1",		   0x11, 0, 0x15, 1, 0x2C, 0, 0x2F, 1, "Zwarcie do Vcc",	     "Zwarcie do GND/Przerwa", &err16f },
	{ ERR_COIL2,	 "Cewka zapłonowa 1",		   0x11, 0, 0x15, 2, 0x2C, 0, 0x2F, 2, "Zwarcie do Vcc",	     "Zwarcie do GND/Przerwa", &err16f },
	{ ERR_IAV,	 "Silnik krokowy biegu jałowego",  0x11, 0, 0x15, 3, 0x2C, 0, 0x2F, 3, "Zwarcie lub zab. termiczne", "Przerwa",		       &err16f },
	{ ERR_EVAP,	 "Zawór EVAP",			   0x11, 0, 0x15, 4, 0x2C, 0, 0x2F, 4, "Zwarcie do Vcc",	     "Zwarcie do GND/Przerwa", &err16f },
	{ ERR_AIR_CO,	 "Klimatyzator",		   0x11, 0, 0x15, 5, 0x2C, 0, 0x2F, 5, "Zwarcie do Vcc",	     "Zwarcie do GND/Przerwa", &err16f },
	{ ERR_FUEL_PUMP, "Pompa paliwa",		   0x11, 0, 0x15, 6, 0x2C, 0, 0x2F, 6, "Zwarcie do Vcc",	     "Zwarcie do GND/Przerwa", &err16f },
	{ ERR_GEN_REL,	 "Przekaźnik główny",		   0x11, 0, 0x15, 7, 0x2C, 0, 0x2F, 7, "Zwarcie do Vcc",	     "Zwarcie do GND/Przerwa", &err16f },
	{ ERR_AC_PARAM,	 "Parametr autokalibracji",	   0x12, 0, 0x16, 0, 0x2D, 0, 0x30, 0, "Max BOGATA",		     "Max UBOGA",	       &err16f },
	{ ERR_RAM,	 "RAM",				   0x12, 0, 0x16, 1, 0x2D, 0, 0x30, 1, "",			     "",		       &err16f },
	{ ERR_ROM,	 "ROM",				   0x12, 0, 0x16, 2, 0x2D, 0, 0x30, 2, "",			     "",		       &err16f },
	{ ERR_EEPROM,	 "EEPROM",			   0x12, 0, 0x16, 3, 0x2D, 0, 0x30, 3, "",			     "",		       &err16f },
	{ ERR_CPU,	 "CPU",				   0x12, 0, 0x16, 4, 0x2D, 0, 0x30, 4, "",			     "",		       &err16f },
	{ ERR_RPM_SENS,	 "Czujnik położenia wału",	   0x12, 0, 0x16, 5, 0x2D, 0, 0x30, 5, "",			     "",		       &err16f },
	{ ERR_F7,	 "Nieznany błąd FUNC (7)",	   0x12, 0, 0x16, 6, 0x2D, 0, 0x30, 6, "",			     "",		       &err16f },
	{ ERR_F8,	 "Nieznany błąd FUNC (8)",	   0x12, 0, 0x16, 7, 0x2D, 0, 0x30, 7, "",			     "",		       &err16f }
};


struct adjustments_map adjustments[] = {
	{ TOGGLE_TRIM_AC, "Włącz/wyłącz autokalibrację składu mieszanki",  0x00, 0x00, 1, 0, "toggle"  },
	{ TOGGLE_STEP_AC, "Włącz/wyłącz autokalibrację silnika krokowego", 0x00, 0x00, 1, 0, "toggle"  },
	{ TRIM_RESET,	  "Reset autokalibracji składu mieszanki",	   0x00, 0x00, 2, 0, "toggle"  },
	{ STEP_RESET,	  "Reset autokalibracji silnika krokowego",	   0x00, 0x00, 2, 0, "toggle"  },
	{ TRIM_SET,	  "Ustaw korektę mieszanki (TRIMRAM)",		   0x0F, 0xFF, 1, 1, "onebyte" },
	{ STEP_SET,	  "Ustaw korektę wolnych obrotów (ALFAFTR)",	   0x29, 0xFF, 1, 1, "onebyte" }
};

struct active_tests_map tests[] = {
	{ FUEL_PUMP,	 "Przekaźnik pompy paliwa", 0, 30 },
	{ INJECTOR,	 "Wtryskiwacz",		    0, 5  },
	{ COIL1,	 "Cewka zapłonowa 1",	    0, 5  },
	{ COIL2,	 "Cewka zapłonowa 2",	    0, 5  },
	{ EVAP,		 "Zawór EVAP",		    0, 7  },
	{ REV_METER,	 "Obrotomierz",		    0, 2  },
	{ AC_RELAY,	 "Przekaźnik klimatyzacji", 0, 30 },
	{ GENERIC_RELAY, "Przekaźnik główny",	    0, 30 }
};

struct engine_info_decode info_decode_functions[] = {
	{ PERIODE,	  &periode	  },
	{ T_INJ_AP,	  &t_inj_ap	  },
	{ AVANCE,	  &avance	  },
	{ MP2_MP8,	  &mp2_mp8	  },
	{ MT_AIR_L,	  &mt_air_l	  },
	{ MT_EAU_L,	  &mt_eau_l	  },
	{ ANG_PAP0,	  &ang_pap0	  },
	{ V_BATT,	  &v_batt	  },
	{ K_O2,		  &k_02		  },
	{ ALFAR,	  &alfar	  },
	{ INTEGR,	  &integr	  },
	{ PROP,		  &prop		  },
	{ TRIMRAM,	  &trimram	  },
	{ ALFAU,	  &alfau	  },
	{ ALFAUC,	  &alfauc	  },
	{ CONS_REG,	  &cons_reg	  },
	{ OFNNTR,	  &ofnntr	  },
	{ DELREG,	  &delreg	  },
	{ ALFAFTR,	  &alfaftr	  },
	{ ADOFFSET,	  &adoffset	  },
	{ ADOFFPURG,	  &adoffpurg	  },
	{ ADOFFPL,	  &adoffpl	  },
	{ ADGAIN,	  &adgain	  },
	{ STA_ENG_RUN,	  &sta_eng_run	  },
	{ STA_SENS_OK,	  &sta_sens_ok	  },
	{ STA_THR_MM,	  &sta_thr_mm	  },
	{ STA_CL_LOOP,	  &sta_cl_loop	  },
	{ STA_AIR_CO_ON,  &sta_air_co_on  },
	{ STA_MIX_ACOK,	  &sta_mix_acok	  },
	{ STA_STEP_ACOK,  &sta_step_acok  },
	{ STA_P_LATCH_OK, &sta_p_latch_ok }
};

struct units_map units[] = {
	{ PERIODE,	  "rpm"	   },
	{ T_INJ_AP,	  "ms"	   },
	{ AVANCE,	  "°"	   },
	{ MP2_MP8,	  "hPa"	   },
	{ MT_AIR_L,	  "°C"	   },
	{ MT_EAU_L,	  "°C"	   },
	{ ANG_PAP0,	  "°"	   },
	{ V_BATT,	  "V"	   },
	{ K_O2,		  ""	   },
	{ ALFAR,	  "kroków" },
	{ INTEGR,	  ""	   },
	{ PROP,		  ""	   },
	{ TRIMRAM,	  "kroków" },
	{ ALFAU,	  ""	   },
	{ ALFAUC,	  ""	   },
	{ CONS_REG,	  "rpm"	   },
	{ OFNNTR,	  "rpm"	   },
	{ DELREG,	  ""	   },
	{ ALFAFTR,	  ""	   },
	{ ADOFFSET,	  ""	   },
	{ ADOFFPURG,	  ""	   },
	{ ADOFFPL,	  ""	   },
	{ ADGAIN,	  ""	   },
	{ STA_ENG_RUN,	  ""	   },
	{ STA_SENS_OK,	  ""	   },
	{ STA_THR_MM,	  ""	   },
	{ STA_CL_LOOP,	  ""	   },
	{ STA_AIR_CO_ON,  ""	   },
	{ STA_MIX_ACOK,	  ""	   },
	{ STA_STEP_ACOK,  ""	   },
	{ STA_P_LATCH_OK, ""	   }
};

struct engine_map engine_info[] = {
	{ PERIODE,	  "Obroty silnika",						     2 },
	{ T_INJ_AP,	  "Czas wtrysku",						     2 },
	{ AVANCE,	  "Wyprzedzenie zapłonu",					     1 },
	{ MP2_MP8,	  "Ciśnienie w kolektorze dolotowym",				     1 },
	{ MT_AIR_L,	  "Temperatura powietrza",					     1 },
	{ MT_EAU_L,	  "Temperatura wody",						     1 },
	{ ANG_PAP0,	  "Kąt otwarcia przepustnicy",					     1 },
	{ V_BATT,	  "Napięcie akumulatora",					     1 },
	{ K_O2,		  "Korekta sondy lambda",					     1 },
	{ ALFAR,	  "Pozycja silnika krokowego",					     1 },
	{ INTEGR,	  "Wzmocnienie członu całkującego regulacji silnika krokowego",	     1 },
	{ PROP,		  "Wzmocnienie członu proporcjonalnego regulacji silnika krokowego", 1 },
	{ TRIMRAM,	  "Korekta dawki paliwa",					     1 },
	{ ALFAU,	  "Korekta autokalibracji",					     1 },
	{ ALFAUC,	  "Korekta autokalibracji z klimatyzacją",			     1 },
	{ CONS_REG,	  "Obroty biegu jałowego",					     1 },
	{ OFNNTR,	  "Korekta obrotów biegu jałowego",				     1 },
	{ DELREG,	  "Korekta silnika krokowego (delta)",				     1 },
	{ ALFAFTR,	  "Korekta silnika krokowego z FLT",				     1 },
	{ ADOFFSET,	  "Offset autokalibracji (obciążenie min., EVAP wył.)",		     2 },
	{ ADOFFPURG,	  "Offset autokalibracji (obciążenie min., EVAP wł.)",		     2 },
	{ ADOFFPL,	  "Offset autokalibracji (obciążenie min.)",			     2 },
	{ ADGAIN,	  "Wzmocnienie autokalibracji (średnie obciążenie)",		     2 },
	{ STA_ENG_RUN,	  "Silnik pracuje",						     1 },
	{ STA_SENS_OK,	  "Sygnały z czujników OK",					     1 },
	{ STA_THR_MM,	  "Przepustnica w skrajnej pozycji",				     1 },
	{ STA_CL_LOOP,	  "Sonda lambda w pętli zamkniętej",				     1 },
	{ STA_AIR_CO_ON,  "Klimatyzacja włączona",					     1 },
	{ STA_MIX_ACOK,	  "Autokalibracja mieszanki WŁĄCZONA",				     1 },
	{ STA_STEP_ACOK,  "Autolakibrajca silnika krokowego WŁĄCZONA",			     1 },
	{ STA_P_LATCH_OK, "Linia zasialnia OK",						     1 }
};

void info_log(enum engine_info_key key) {
	fputs(engine_info_find(key), stdout);
}

void error_log(enum engine_error_key key) {
	fputs(engine_error_find(key), stdout);
}

static const char *engine_error_find(enum engine_error_key key) {
	for (int i = 0; i < ENGINE_ERRORS_SIZE; i++)
		if (key == engine_errors[i].key)
			return engine_errors[i].description;
}

static const char *engine_info_find(enum engine_info_key key) {
	for (int i = 0; i < INFO_SIZE; i++)
		if (key == engine_info[i].key)
			return engine_info[i].description;
}

/* Engine speed */
static double periode(uint8_t *buffer) {
	if (buffer[1] | buffer[2] == 0)
		return 0;
	else if (buffer[1] | buffer[2] == 255)
		return 0;
	else
		return (double)(15e+6 / (buffer[1] << 8 | buffer[2]));
}

/*Injection duration*/
static double t_inj_ap(uint8_t *buffer) {
	return (double)((buffer[3] << 8 | buffer[4]) / 500);
}

/*Ignition advance*/
static double avance(uint8_t *buffer) {
	return (double)(buffer[5] / 2);
}

/*Manifold absolute pressure*/
static double mp2_mp8(uint8_t *buffer) {
	return (double)(buffer[6] * 4);
}

/*Air temperature*/
static double mt_air_l(uint8_t *buffer) {
	return (double)(buffer[7] - 40);
}

/*Water temperature*/
static double mt_eau_l(uint8_t *buffer) {
	return (double)(buffer[8] - 40);
}

/*Throttle angle*/
static double ang_pap0(uint8_t *buffer) {
	return (double)(buffer[9] * 0.4234 - 2.9638);
}

/*Battery voltage*/
static double v_batt(uint8_t *buffer) {
	return (double)(buffer[0x0A] * 0.625);
}

/*Lambda Probe Correction*/
static double k_02(uint8_t *buffer) {
	return (double)(buffer[0x0B] * 0.002656 + 0.66);
}

/*Idle Stepper Motor Position*/
static double alfar(uint8_t *buffer) {
	return (double)(buffer[0x0C]);
}

/*Idle Stepper Integral Gain (2's complement)*/
static double integr(uint8_t *buffer) {
	return (double)((int8_t)buffer[0x0D]);
}

/*Idle Stepper Proportional Gain (2's complement)*/
static double prop(uint8_t *buffer) {
	return (double)((int8_t)buffer[0x0E]);
}

/*Trimmer Position*/
static double trimram(uint8_t *buffer) {
	return (double)(buffer[0x0F] - 128);
}

/*AutoCalibration Correction*/
static double alfau(uint8_t *buffer) {
	return (double)((int16_t)((uint16_t)buffer[0x22]) << 8 | (uint16_t)buffer[0x23]);
}

/*AutoCalibration Correction with A/C*/
static double alfauc(uint8_t *buffer) {
	return (double)((int16_t)((uint16_t)buffer[0x24]) << 8 | (uint16_t)buffer[0x25]);
}

/*Minimum Engine Speed (Desired)*/
static double cons_reg(uint8_t *buffer) {
	return (double)(buffer[0x26] * 8);
}

/*Minimum Offset turns*/
static double ofnntr(uint8_t *buffer) {
	return (double)((buffer[0x27] - 128) * 8);
}

/*Stepper Correction (Delta)*/
static double delreg(uint8_t *buffer) {
	return (double)(buffer[0x28] - 128);
}

/*Stepper Correction from FLT*/
static double alfaftr(uint8_t *buffer) {
	return (double)(buffer[0x29] - 128);
}

/*AC Offset Parameter (Minimum with Canister deactivated)*/
static double adoffset(uint8_t *buffer) {
	return (double)((int16_t)((uint16_t)buffer[0x32]) << 8 | (uint16_t)buffer[0x33]);
}

/*AC Offset Parameter (Minimum with Canister activated)*/
static double adoffpurg(uint8_t *buffer) {
	return (double)((int16_t)((uint16_t)buffer[0x34]) << 8 | (uint16_t)buffer[0x35]);
}

/*AC Offset Parameter (Minimum)*/
static double adoffpl(uint8_t *buffer) {
	return (double)((int16_t)((uint16_t)buffer[0x36]) << 8 | (uint16_t)buffer[0x37]);
}

/*AC Gain Parameter (Midrange value)*/
static double adgain(uint8_t *buffer) {
	return (double)((int16_t)((uint16_t)buffer[0x38]) << 8 | (uint16_t)buffer[0x39]);
}

/*Engine Running*/
static double sta_eng_run(uint8_t *buffer) {
	return (double)(buffer[0x13] & 1 != 0);
}

/*Signals OK*/
static double sta_sens_ok(uint8_t *buffer) {
	return (double)((buffer[0x13] & 2) != 0);
}

/*Throttle Min/Max*/
static double sta_thr_mm(uint8_t *buffer) {
	return (double)((buffer[0x13] & 3) != 0);
}

/*Closed Loop*/
static double sta_cl_loop(uint8_t *buffer) {
	return (double)((buffer[0x13] & 4) != 0);
}

/*Air Conditioner ON*/
static double sta_air_co_on(uint8_t *buffer) {
	return (double)((buffer[0x13] & 5) != 0);
}

/*System Autocalibration OK*/
static double sta_mix_acok(uint8_t *buffer) {
	return (double)((buffer[0x13] & 6) != 0);
}

/*Stepper Autocalibration OK*/
static double sta_step_acok(uint8_t *buffer) {
	return (double)((buffer[0x2A] & 2) != 0);
}

/*Power-Latch OK*/
static double sta_p_latch_ok(uint8_t *buffer) {
	return (double)((buffer[0x2A] & 6) == 0);
}

static struct error_state *err16f(struct error_element *element, uint8_t *buffer) {
	element->state.is_active = buffer[element->ra_base] & element->r_base ? 1 : 0;
	element->state.is_stored = buffer[element->rs_base] & element->r_base ? 1 : 0;
	element->state.reason = buffer[element->rs_base] & element->r_base ?
				buffer[element->rs_ext] & element->rb_ext : buffer[element->ra_ext] & element->rb_ext ?
				element->h_ext : element->l_ext;

	return &(element->state);
}

static struct error_state *err_code(struct error_element *element, uint8_t *buffer) {
	element->state.is_active = buffer[element->ra_base] & element->r_base ? 1 : 0;
	element->state.is_stored = buffer[element->rs_base] & element->r_base ? 1 : 0;
	element->state.reason = "FIAT CODE";

	return &(element->state);
}
