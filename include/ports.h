#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

void out(uint16_t port, uint8_t value);
uint8_t in(uint16_t port);

#endif
