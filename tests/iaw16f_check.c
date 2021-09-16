#include <check.h>
#include "iaw16f_check.h"
#include "../src/libs/iaw16f/iaw16f.h"

START_TEST(init_ecu_test) {
	struct iaw16f *ecu;

	init_ecu(ecu);

	ck_assert_str_eq(ecu->name, NAME);
	ck_assert_str_eq(ecu->long_name, LONG_NAME);

	free_ecu(ecu);
}
END_TEST

int main() {
	return 0;
}
