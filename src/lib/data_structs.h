#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stdint.h>

struct data_element {
	char	*description;
	char	*unit;
	char	*value;
	uint8_t *request;
};

struct test_element {
	char	*description;
	uint8_t *request_set;
	uint8_t time_out;
	int	is_engine_on;
};

struct adjust_element {
	uint8_t *pre_set;
	uint8_t *post_set;
	char	*description;
	uint8_t *status;
	uint8_t *status_mask;
	char	*type;
};

struct error_state {
	int	is_active;
	int	is_verified;
	int	is_stored;
	uint8_t count;
	char	*reason;
};

struct error_element {
	char			*description;
	struct error_state	state;

	uint8_t			ra_base;
	uint8_t			rv_base;
	uint8_t			rs_base;
	uint8_t			r_base;
	uint8_t			ra_ext;
	uint8_t			rv_ext;
	uint8_t			rs_ext;
	uint8_t			rb_ext;
	char			*h_ext;
	char			*l_ext;
};

struct state_element {
	char	*description;
	int	value;
};

struct query_result {
	uint8_t request;
	uint8_t response;
	int	is_success;
};

#endif
