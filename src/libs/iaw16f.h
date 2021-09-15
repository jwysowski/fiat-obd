#ifndef IAW16F_H
#define IAW16F_H

#include "data_structs.h"

struct iaw16f {
	const char	*name = "IAW-6F/16F";
	const char	*long_name = "Magneti-Marelli IAW-6F/16F SPI";
	const char	*car[2] = {
		"55D085139754",
		"Seicento 0.9 CM SPI F2"
	};
	int		has_immo;
	char		*iso;
	char		*codric;
	const int	init_baud;
	const int	comm_baud;
	int		is_connected;
	char		buffer[255];
	int		valid[255];

	data_element	engine_data[30];
	test_element	active_tests[7];
	adjust_element	adjustments[5];
	error_element	engine_errors[23];
	error_element	immo_errors[7];

	char		engine_err_req[12];
	char		immo_err_req[2];
	test_element	clear_codes;
};


#endif
