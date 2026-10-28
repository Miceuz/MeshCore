#pragma once

#include <variant_generic.h>

#undef RNG

#ifndef FLASH_END_ADDR
  #define FLASH_END_ADDR (FLASH_BASE + (NORDUVA_RX_FLASH_KB * 1024UL) - 1)
#endif
