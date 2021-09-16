#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

struct data_element {
	char	*description;
	char	*unit;
	char	*value;
	char	*request;
};

struct test_element {
	char	request_set;
	char	*description;
	char	time_out;
	int	is_engine_on;
};

struct adjust_element {
	char	*pre_set;
	char	*post_set;
	char	*description;
	char	*status;
	char	*status_mask;
	char	*type;
};

struct error_state {
	int	is_active;
	int	is_verified;
	int	is_stored;
	char	count;
	char	*reason;
};

struct error_element {
	char			*description;
	struct error_state	state;
	char			ra_base;
	char			rv_base;
	char			rs_base;
	char			r_base;
	char			ra_ext;
	char			rv_ext;
	char			rs_ext;
	char			rb_ext;
	char			*h_ext;
	char			*l_ext;
};

struct state_element {
	char	*description;
	int	value;
};

struct query_result {
	char	request;
	char	response;
	int	is_success;
};

#endif
