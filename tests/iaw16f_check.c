#include <stdlib.h>
#include <check.h>
#include "../src/lib/iaw16f/iaw16f.h"

START_TEST(init_ecu_test) {
	struct iaw16f *ecu;

	init_ecu(ecu);
	ck_assert_str_eq(ecu->name, NAME);
	ck_assert_str_eq(ecu->long_name, LONG_NAME);
	ck_assert_str_eq(ecu->car[0], CAR_0);
	ck_assert_int_eq(ecu->has_immo, HAS_IMMO);
	ck_assert_str_eq(ecu->iso, ISO);
	ck_assert_str_eq(ecu->codric, CODRIC);
	ck_assert_int_eq(ecu->init_baud, INIT_BAUD);
	ck_assert_int_eq(ecu->comm_baud, COMM_BAUD);
	ck_assert_int_eq(ecu->is_connected, IS_CONNECTED);
	ck_assert_int_eq(ecu->immo_err_req[0], IMMO_ERR_REQ_0);
	// ck_assert_str_eq(ecu->engine_data[0].description, PERIODE);
	// ck_assert_str_eq(ecu->engine_errors[0].description, ERR_TPS);
	// ck_assert_str_eq(ecu->adjustments[0].description, TOGGLE_TRIM_AC);
	// ck_assert_str_eq(ecu->active_tests[0].description, FUEL_PUMP);
	// ck_assert_str_eq(ecu->immo_errors[0].description, ERR_NO_SYNC);
	ck_assert_str_eq(ecu->clear_codes.description, "");

	free_ecu(ecu);
}
END_TEST

Suite *iaw16f_suite();

int main() {
	int failed;
	Suite *s;
	SRunner *sr;

	s = iaw16f_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_VERBOSE);
	failed = srunner_ntests_failed(sr);

	srunner_free(sr);

	return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

Suite *iaw16f_suite() {
	Suite *s;
	TCase *tc_core;

	s = suite_create("IAW16F");

	/*Core test case*/
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, init_ecu_test);
	suite_add_tcase(s, tc_core);

	return s;
}
