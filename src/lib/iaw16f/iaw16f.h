#ifndef IAW16F_H
#define IAW16F_H

#include "../data_structs.h"

#define ENGINE_DATA_SIZE        31
#define BUFFER_VALID_SIZE       255
#define ACTIVE_TESTS_SIZE       8
#define ADJUSTMENTS_SIZE        6
#define ENGINE_ERRORS_SIZE      24
#define IMMO_ERRORS_SIZE        8
#define CAR_SIZE                2
#define ENGINE_ERR_REQ_SIZE     12
#define IMMO_ERR_REQ_SIZE       2

#define HAS_IMMO                0
#define ISO                     "iso"
#define CODRIC                  "CODRIC"
#define INIT_BAUD               1200
#define COMM_BAUD               7680
#define IS_CONNECTED            0
#define IMMO_ERR_REQ_0          0x71
#define IMMO_ERR_REQ_1          0x72
#define NAME                    "IAW-6F/16F"
#define LONG_NAME               "Magneti-Marelli IAW-6F/16F SPI"
#define CAR_0                   "55D085139754"
#define CAR_1                   "Seicento 0.9 CM SPI F2"

struct iaw16f {
	const char		*name;
	const char		*long_name;
	const char		*car[CAR_SIZE];
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
	struct error_element	engine_errors[ENGINE_ERRORS_SIZE];
	struct error_element	immo_errors[IMMO_ERRORS_SIZE];

	uint8_t			engine_err_req[ENGINE_ERR_REQ_SIZE];
	uint8_t			immo_err_req[IMMO_ERR_REQ_SIZE];
	struct test_element	clear_codes;
};

void init_ecu(struct iaw16f *ecu);
void free_ecu(struct iaw16f *ecu);

#endif
