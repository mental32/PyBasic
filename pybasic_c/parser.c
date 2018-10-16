#include "stdint.h"
#include "instructions.h"

void _parse_source_to_bytecode(uint8_t *buf, const char *source) {
	buf[0] = (uint8_t) _INS_RETURN;
	return;
}
