#include <stdio.h>
#include <stdint.h>
#include "data.h"

static const char *error_find(enum key_error key);
static const char *info_find(enum key_info key);

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

struct decode_info_map functions[] = {
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

struct units_info_map units[] = {
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

struct engine_info_map engine_info[] = {
	{ PERIODE,	  "Obroty silnika",						     2, NULL },
	{ T_INJ_AP,	  "Czas wtrysku",						     2, NULL },
	{ AVANCE,	  "Wyprzedzenie zapłonu",					     1, NULL },
	{ MP2_MP8,	  "Ciśnienie w kolektorze dolotowym",				     1, NULL },
	{ MT_AIR_L,	  "Temperatura powietrza",					     1, NULL },
	{ MT_EAU_L,	  "Temperatura wody",						     1, NULL },
	{ ANG_PAP0,	  "Kąt otwarcia przepustnicy",					     1, NULL },
	{ V_BATT,	  "Napięcie akumulatora",					     1, NULL },
	{ K_O2,		  "Korekta sondy lambda",					     1, NULL },
	{ ALFAR,	  "Pozycja silnika krokowego",					     1, NULL },
	{ INTEGR,	  "Wzmocnienie członu całkującego regulacji silnika krokowego",	     1, NULL },
	{ PROP,		  "Wzmocnienie członu proporcjonalnego regulacji silnika krokowego", 1, NULL },
	{ TRIMRAM,	  "Korekta dawki paliwa",					     1, NULL },
	{ ALFAU,	  "Korekta autokalibracji",					     1, NULL },
	{ ALFAUC,	  "Korekta autokalibracji z klimatyzacją",			     1, NULL },
	{ CONS_REG,	  "Obroty biegu jałowego",					     1, NULL },
	{ OFNNTR,	  "Korekta obrotów biegu jałowego",				     1, NULL },
	{ DELREG,	  "Korekta silnika krokowego (delta)",				     1, NULL },
	{ ALFAFTR,	  "Korekta silnika krokowego z FLT",				     1, NULL },
	{ ADOFFSET,	  "Offset autokalibracji (obciążenie min., EVAP wył.)",		     2, NULL },
	{ ADOFFPURG,	  "Offset autokalibracji (obciążenie min., EVAP wł.)",		     2, NULL },
	{ ADOFFPL,	  "Offset autokalibracji (obciążenie min.)",			     2, NULL },
	{ ADGAIN,	  "Wzmocnienie autokalibracji (średnie obciążenie)",		     2, NULL },
	{ STA_ENG_RUN,	  "Silnik pracuje",						     1, NULL },
	{ STA_SENS_OK,	  "Sygnały z czujników OK",					     1, NULL },
	{ STA_THR_MM,	  "Przepustnica w skrajnej pozycji",				     1, NULL },
	{ STA_CL_LOOP,	  "Sonda lambda w pętli zamkniętej",				     1, NULL },
	{ STA_AIR_CO_ON,  "Klimatyzacja włączona",					     1, NULL },
	{ STA_MIX_ACOK,	  "Autokalibracja mieszanki WŁĄCZONA",				     1, NULL },
	{ STA_STEP_ACOK,  "Autolakibrajca silnika krokowego WŁĄCZONA",			     1, NULL },
	{ STA_P_LATCH_OK, "Linia zasialnia OK",						     1, NULL }
};

struct error_map errors[] = {
	{ NO_SYNC_ERR,	 "0" },
	{ START_DIS_ERR, "0" },
	{ UNI_CODE_ERR,	 "0" }
};

void info_log(enum key_info key) {
	fputs(info_find(key), stdout);
}

void error_log(enum key_error key) {
	fputs(error_find(key), stdout);
}

static const char *error_find(enum key_error key) {
	for (int i = 0; i < sizeof(errors) / sizeof(errors[0]); i++)
		if (key == errors[i].key)
			return errors[i].value;
}

static const char *engine_info_find(enum key_info key) {
	for (int i = 0; i < sizeof(engine_info) / sizeof(engine_info[0]); i++)
		if (key == engine_info[i].key)
			return engine_info[i].value;
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
