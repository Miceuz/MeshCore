#pragma once

#define RADIOLIB_STATIC_ONLY 1
#include <RadioLib.h>
#include <helpers/radiolib/RadioLibWrappers.h>
#include <helpers/stm32/STM32Board.h>
#include <helpers/radiolib/CustomSX1276Wrapper.h>
#include <helpers/ArduinoHelpers.h>
#include <helpers/SensorManager.h>

class NorduvaRXBoard : public STM32Board {
public:
  void begin() override {
    STM32Board::begin();

    pinMode(P_LORA_TX_LED, OUTPUT);
    digitalWrite(P_LORA_TX_LED, LOW);
    pinMode(PIN_USER_BTN, INPUT_PULLUP);
  }

  const char* getManufacturerName() const override {
    return "Norduva RX SX1276";
  }

  // TX LED is active high on this board
  void onBeforeTransmit() override {
    digitalWrite(P_LORA_TX_LED, HIGH);
  }
  void onAfterTransmit() override {
    digitalWrite(P_LORA_TX_LED, LOW);
  }
};

extern NorduvaRXBoard board;
extern WRAPPER_CLASS radio_driver;
extern VolatileRTCClock rtc_clock;
extern SensorManager sensors;

bool radio_init();
uint32_t radio_get_rng_seed();
void radio_set_params(float freq, float bw, uint8_t sf, uint8_t cr);
void radio_set_tx_power(int8_t dbm);
mesh::LocalIdentity radio_new_identity();
