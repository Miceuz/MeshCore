#include <Arduino.h>
#include "target.h"

NorduvaRXBoard board;

RADIO_CLASS radio = new Module(P_LORA_NSS, P_LORA_DIO_0, P_LORA_RESET, P_LORA_DIO_1, SPI);

WRAPPER_CLASS radio_driver(radio, board);

VolatileRTCClock rtc_clock;
SensorManager sensors;

#define SX127X_REG_TCXO        0x4B
#define SX127X_TCXO_INPUT_ON   0x10

bool radio_init() {
  if (!radio.std_init(&SPI)) return false;

  // 32 MHz clipped-sine TCXO is AC-coupled into XTA: switch XO to external input (set in sleep)
  radio.sleep();
  radio.getMod()->SPIsetRegValue(SX127X_REG_TCXO, SX127X_TCXO_INPUT_ON, 4, 4);
  radio.standby();

  return true;
}

uint32_t radio_get_rng_seed() {
  return radio.random(0x7FFFFFFF);
}

void radio_set_params(float freq, float bw, uint8_t sf, uint8_t cr) {
  radio.setFrequency(freq);
  radio.setSpreadingFactor(sf);
  radio.setBandwidth(bw);
  radio.setCodingRate(cr);
}

void radio_set_tx_power(int8_t dbm) {
  radio.setOutputPower(dbm);
}

mesh::LocalIdentity radio_new_identity() {
  RadioNoiseListener rng(radio);
  return mesh::LocalIdentity(&rng);  // create new random identity
}
