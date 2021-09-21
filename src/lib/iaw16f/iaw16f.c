#include <stdlib.h>
#include <stdio.h>
#include "iaw16f.h"
#include "../data/data.h"

static void init_buffer(struct iaw16f *ecu);
static void init_engine_data(struct iaw16f *ecu);
static void init_tests(struct iaw16f *ecu);
static void init_adjustments(struct iaw16f *ecu);
static void init_engine_errors(struct iaw16f *ecu);
static void init_immo_errors(struct iaw16f *ecu);
static void init_engine_err_req(struct iaw16f *ecu);
static void init_clear_codes(struct iaw16f *ecu);

void init_ecu(struct iaw16f *ecu) {
	ecu->name = NAME;
	ecu->long_name = LONG_NAME;
	ecu->car[0] = CAR_0;
	ecu->car[1] = CAR_1;
	ecu->has_immo = HAS_IMMO;
	ecu->iso = ISO;
	ecu->codric = CODRIC;
	ecu->init_baud = INIT_BAUD;
	ecu->comm_baud = COMM_BAUD;
	ecu->is_connected = IS_CONNECTED;
	ecu->immo_err_req[0] = IMMO_ERR_REQ_0;
	ecu->immo_err_req[1] = IMMO_ERR_REQ_1;

	init_buffer(ecu);
	init_engine_data(ecu);
	init_tests(ecu);
	init_adjustments(ecu);
	init_engine_errors(ecu);
	init_immo_errors(ecu);
	init_engine_err_req(ecu);
	init_clear_codes(ecu);
}

static void init_immo_errors(struct iaw16f *ecu) {
	for (int i = 0; i < IMMO_ERRORS_SIZE; i++) {
		ecu->immo_errors[i].description = immo_errors[i].description;

		ecu->immo_errors[i].ra_base = immo_errors[i].ra_base;
		ecu->immo_errors[i].rv_base = immo_errors[i].rv_base;
		ecu->immo_errors[i].rs_base = immo_errors[i].rs_base;
		ecu->immo_errors[i].r_base = immo_errors[i].r_base;
		ecu->immo_errors[i].ra_ext = immo_errors[i].ra_ext;
		ecu->immo_errors[i].rv_ext = immo_errors[i].rv_ext;
		ecu->immo_errors[i].rs_ext = immo_errors[i].rs_ext;
		ecu->immo_errors[i].rb_ext = immo_errors[i].rb_ext;

		ecu->immo_errors[i].h_ext = immo_errors[i].h_ext;
		ecu->immo_errors[i].l_ext = immo_errors[i].l_ext;

		ecu->immo_errors[i].state = *(*(immo_errors[i].err_decode))(&(ecu->immo_errors[i]), ecu->buffer);
	}
}

static void init_clear_codes(struct iaw16f *ecu) {
	ecu->clear_codes.description = "";
	ecu->clear_codes.is_engine_on = 0;
	ecu->clear_codes.time_out = 10;
	ecu->clear_codes.request_set[0] = 0x84;
}

static void init_engine_err_req(struct iaw16f *ecu) {
	ecu->engine_err_req[0] = 0x10;
	ecu->engine_err_req[1] = 0x11;
	ecu->engine_err_req[2] = 0x12;
	ecu->engine_err_req[3] = 0x14;
	ecu->engine_err_req[4] = 0x15;
	ecu->engine_err_req[5] = 0x16;
	ecu->engine_err_req[6] = 0x2B;
	ecu->engine_err_req[7] = 0x2C;
	ecu->engine_err_req[8] = 0x2D;
	ecu->engine_err_req[9] = 0x2E;
	ecu->engine_err_req[10] = 0x2F;
	ecu->engine_err_req[11] = 0x30;
}

static void init_buffer(struct iaw16f *ecu) {
	for (int i = 0; i < BUFFER_VALID_SIZE; i++) {
		ecu->buffer[i] = i;
		ecu->valid[i] = i % 2;
	}
}

static void init_engine_data(struct iaw16f *ecu) {
	uint8_t request_init = 1;

	for (int i = 0; i < ENGINE_DATA_SIZE; i++) {
		ecu->engine_data[i].description = engine_info[i].description;
		ecu->engine_data[i].value = "INIT";
		ecu->engine_data[i].unit = units[i].value;

		ecu->engine_data[i].request = (uint8_t *)malloc(sizeof(uint8_t) * engine_info[i].number_of_bytes);

		if (ecu->engine_data[i].request == NULL) {
			perror("init_engine_data");
			exit(EXIT_FAILURE);
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

		ecu->engine_data[i].val_decode = (*(info_decode_functions[i].info_decode))(ecu->buffer);
	}
}

static void init_tests(struct iaw16f *ecu) {
	for (int i = 0; i < ACTIVE_TESTS_SIZE; i++) {
		ecu->active_tests[i].description = tests[i].description;
		ecu->active_tests[i].is_engine_on = tests[i].is_engine_on;
		ecu->active_tests[i].time_out = tests[i].timeout;

		ecu->active_tests[i].request_set = (uint8_t *)malloc(sizeof(uint8_t));

		if (ecu->active_tests[i].request_set == NULL) {
			perror("init_tests");
			exit(EXIT_FAILURE);
		}

		if (i < 4)
			*(ecu->active_tests[i].request_set) = i + 0x80;
		else
			*(ecu->active_tests[i].request_set) = i + 0x80 + 1;
	}
}

static void init_adjustments(struct iaw16f *ecu) {
	for (int i = 0; i < ADJUSTMENTS_SIZE; i++) {
		ecu->adjustments[i].description = adjustments[i].description;
		ecu->adjustments[i].status = adjustments[i].status;
		ecu->adjustments[i].status_mask = adjustments[i].status_mask;

		ecu->adjustments[i].pre_set = (uint8_t *)malloc(sizeof(uint8_t) * adjustments[i].pre_set_number_of_bytes);

		if (ecu->adjustments[i].pre_set == NULL) {
			perror("init_adjustments pre_set");
			exit(EXIT_FAILURE);
		}

		if (adjustments[i].post_set_number_of_bytes != 0) {
			ecu->adjustments[i].post_set = (uint8_t *)malloc(sizeof(uint8_t) * adjustments[i].post_set_number_of_bytes);

			if (ecu->adjustments[i].post_set == NULL) {
				perror("init_adjustments post_set");
				exit(EXIT_FAILURE);
			}
		}

		ecu->adjustments[i].type = adjustments[i].type;
	}
}

static void init_engine_errors(struct iaw16f *ecu) {
	for (int i = 0; i < ENGINE_ERRORS_SIZE; i++) {
		ecu->engine_errors[i].description = engine_errors[i].description;

		ecu->engine_errors[i].ra_base = engine_errors[i].ra_base;
		ecu->engine_errors[i].rv_base = engine_errors[i].rv_base;
		ecu->engine_errors[i].rs_base = engine_errors[i].rs_base;
		ecu->engine_errors[i].r_base = engine_errors[i].r_base;
		ecu->engine_errors[i].ra_ext = engine_errors[i].ra_ext;
		ecu->engine_errors[i].rv_ext = engine_errors[i].rv_ext;
		ecu->engine_errors[i].rs_ext = engine_errors[i].rs_ext;
		ecu->engine_errors[i].rb_ext = engine_errors[i].rb_ext;

		ecu->engine_errors[i].h_ext = engine_errors[i].h_ext;
		ecu->engine_errors[i].l_ext = engine_errors[i].l_ext;

		ecu->engine_errors[i].state = *(*(engine_errors[i].err_decode))(&(ecu->engine_errors[i]), ecu->buffer);
	}
}

void free_ecu(struct iaw16f *ecu) {
	for (int i = 0; i < ENGINE_DATA_SIZE; i++) {
		free(ecu->engine_data[i].request);
		free(ecu->active_tests[i].request_set);
	}

	free(ecu);
}
