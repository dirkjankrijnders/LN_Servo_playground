#include "bus_configuredpins.h"

TLC5947pin::TLC5947pin(Adafruit_TLC5947* _tlc, uint8_t confpin, uint8_t pin, uint16_t address, bool cumulative, uint8_t _channel, uint16_t _intensity) : OutputPin(confpin, address, cumulative), tlc(_tlc),  channel(_channel), intensity(_intensity) {
};

void TLC5947pin::_set(bool state) {
  tlc->setPWM(channel, state*intensity);
};