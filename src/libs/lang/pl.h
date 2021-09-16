#ifndef PL_H
#define PL_H

enum key_info {
	ACRELAY,
	ADGAIN,
	ADOFFPL,
	ADOFFPURG
};

enum key_error {
	NO_SYNC_ERR,
	START_DIS_ERR,
	UNI_CODE_ERR
};

struct info_map {
	enum key_info	key;
	char		*message;
};

struct error_map {
	enum key_error	key;
	char		*message;
};

#endif
