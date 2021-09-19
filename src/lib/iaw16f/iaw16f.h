#ifndef IAW16F_H
#define IAW16F_H

#include "../data_structs.h"

#define ENGINE_DATA_SIZE        31
#define BUFFER_VALID_SIZE       255
#define ACTIVE_TESTS_SIZE       7
#define ADJUSTMENTS_SIZE        5

struct iaw16f {
	const char		*name;
	const char		*long_name;
	const char		*car[2];
	int			has_immo;
	char			*iso;
	char			*codric;
	int			init_baud;
	int			comm_baud;
	int			is_connected;
	uint8_t			buffer[BUFFER_VALID_SIZE];
	uint8_t			valid[BUFFER_VALID_SIZE];

	struct data_element	engine_data[ENGINE_DATA_SIZE];
	struct test_element	active_tests[ACTIVE_TESTS_SIZE];
	struct adjust_element	adjustments[ADJUSTMENTS_SIZE];
	struct error_element	engine_errors[23];
	struct error_element	immo_errors[7];

	uint8_t			engine_err_req[12];
	uint8_t			immo_err_req[2];
	struct test_element	clear_codes;
};

void init_ecu(struct iaw16f *ecu);
void free_ecu(struct iaw16f *ecu);

#endif
