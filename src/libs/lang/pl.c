#include <stdio.h>
#include "pl.h"

static const char *error_find(enum key_error key);
static const char *info_find(enum key_info key);

struct info_map info[] = {
	{ ACRELAY,   "" },
	{ ADGAIN,    "" },
	{ ADOFFPL,   "" },
	{ ADOFFPURG, "" }
};

struct error_map errors[] = {
	{ NO_SYNC_ERR,	 "" },
	{ START_DIS_ERR, "" },
	{ UNI_CODE_ERR,	 "" }
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
			return errors[i].message;
}

static const char *info_find(enum key_info key) {
	for (int i = 0; i < sizeof(info) / sizeof(info[0]); i++)
		if (key == info[i].key)
			return info[i].message;
}
