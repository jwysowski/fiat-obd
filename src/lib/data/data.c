#include <stdio.h>
#include <stdint.h>
#include "data.h"

static const char *error_find(enum error_key key);
static const char *info_find(enum engine_info_key key);

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

// struct adjustments_map adjustments[] = {
// 	{ TOGGLE_TRIM_AC, }
// 	{ TOGGLE_STEP_AC, }
// 	{ TRIM_RESET,	  }
// 	{ STEP_RESET,	  }
// 	{ TRIM_SET,	  }
// 	{ STEP_SET,	  }
// };

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

struct decode_map functions[] = {
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



struct errors_map errors[] = {
	{ NO_SYNC_ERR,	 "0" },
	{ START_DIS_ERR, "0" },
	{ UNI_CODE_ERR,	 "0" }
};

void info_log(enum engine_info_key key) {
	fputs(info_find(key), stdout);
}

void error_log(enum error_key key) {
	fputs(error_find(key), stdout);
}

static const char *error_find(enum error_key key) {
	for (int i = 0; i < sizeof(errors) / sizeof(errors[0]); i++)
		if (key == errors[i].key)
			return errors[i].description;
}

static const char *engine_info_find(enum engine_info_key key) {
	for (int i = 0; i < sizeof(engine_info) / sizeof(engine_info[0]); i++)
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
