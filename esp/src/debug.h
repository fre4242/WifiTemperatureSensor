#ifndef DEBUG_H
#define DEBUG_H

#include "FormattingSerialDebug.h"

#ifdef DEBUG
#define SERIAL_DEBUG true
#else
#define SERIAL_DEBUG false
#endif

void DEBUG_DUMP_BUFFER(unsigned char *buf, int size);

#endif
