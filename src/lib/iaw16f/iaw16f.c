#include <stdlib.h>
#include <stdio.h>
#include "iaw16f.h"
#include "../data/data.h"

// struct iaw16f {
// 	const char		*name; +
// 	const char		*long_name; +
// 	const char		*car[2]; +
// 	int			has_immo; +
// 	char			*iso; +
// 	char			*codric; +
// 	int			init_baud; +
// 	int			comm_baud; +
// 	int			is_connected; +
// 	char			buffer[255]; +
// 	int			valid[255]; +

// 	struct data_element	engine_data[30]; +
// 	struct test_element	active_tests[7]; +
// 	struct adjust_element	adjustments[5];
// 	struct error_element	engine_errors[23];
// 	struct error_element	immo_errors[7];

// 	char			engine_err_req[12];
// 	char			immo_err_req[2];
// 	struct test_element	clear_codes;
// };

void init_ecu(struct iaw16f *ecu) {
	uint8_t request_init = 1;

	ecu->name = "IAW-6F/16F";
	ecu->long_name = "Magneti-Marelli IAW-6F/16F SPI";
	ecu->car[0] = "55D085139754";
	ecu->car[1] = "Seicento 0.9 CM SPI F2";
	ecu->has_immo = 0;
	ecu->iso = "iso";
	ecu->codric = "codric";
	ecu->init_baud = 1200;
	ecu->comm_baud = 7680;
	ecu->is_connected = 1;

	for (int i = 0; i < BUFFER_VALID_SIZE; i++) {
		ecu->buffer[i] = i;
		ecu->valid[i] = i % 2;
	}

	for (int i = 0; i < ENGINE_DATA_SIZE; i++) {
		ecu->engine_data[i].description = engine_info[i].description;
		ecu->engine_data[i].value = "INIT";
		ecu->engine_data[i].unit = units[i].value;

		ecu->engine_data[i].request = (uint8_t *)malloc(sizeof(uint8_t) * engine_info[i].number_of_bytes);

		if (ecu->engine_data[i].request == NULL) {
			perror(EXIT_FAILURE);
			exit(1);
		}

		if (i < 13) {
			for (int j = 0; j < engine_info[i].number_of_bytes; j++)
				ecu->engine_data[i].request[j] = request_init++;
		} else if (i >= 13 && i < 23) {
			request_init = 0x22;

			for (int j = 0; j < engine_info[i].number_of_bytes; j++)
				ecu->engine_data[i].request[j] = request_init++;
		} else if (i >= 23 && i < 29) {
			request_init = 0x13;

			for (int j = 0; j < engine_info[i].number_of_bytes; j++)
				ecu->engine_data[i].request[j] = request_init;
		} else {
			request_init = 0x2A;

			for (int j = 0; j < engine_info[i].number_of_bytes; j++)
				ecu->engine_data[i].request[j] = request_init;
		}

		ecu->engine_data[i].val_decode = (*(functions[i].function))(ecu->buffer);
	}

	for (int i = 0; i < ACTIVE_TESTS_SIZE; i++) {
		ecu->active_tests[i].description = tests[i].description;
		ecu->active_tests[i].is_engine_on = tests[i].is_engine_on;
		ecu->active_tests[i].time_out = tests[i].timeout;

		ecu->active_tests[i].request_set = (uint8_t *)malloc(sizeof(uint8_t));

		if (ecu->active_tests[i].request_set == NULL) {
			perror(EXIT_FAILURE);
			exit(1);
		}

		if (i < 4)
			*(ecu->active_tests[i].request_set) = i + 0x80;
		else
			*(ecu->active_tests[i].request_set) = i + 0x80 + 1;
	}

	for (int i = 0; i < ADJUSTMENTS_SIZE; i++) {
	}
}

void free_ecu(struct iaw16f *ecu) {
	for (int i = 0; i < ENGINE_DATA_SIZE; i++) {
		free(ecu->engine_data[i].request);
		free(ecu->active_tests[i].request_set);
	}

	free(ecu);
}
