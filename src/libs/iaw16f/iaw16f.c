#include <stdlib.h>
#include "iaw16f.h"

void init_ecu(struct iaw16f *ecu) {
	ecu->name = "IAW-6F/16F";
	ecu->long_name = "Magneti-Marelli IAW-6F/16F SPI";
}

void free_ecu(struct iaw16f *ecu) {
	free(ecu);
}
