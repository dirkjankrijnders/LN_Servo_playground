#include "bus_configuredpins.h"

TLC5947pin::TLC5947pin(Adafruit_TLC5947* _tlc, uint8_t confpin, uint8_t pin, uint16_t address, bool cumulative, uint8_t _channel, uint16_t _intensity) : OutputPin(confpin, pin, address, cumulative), tlc(_tlc), intensity(_intensity), channel(_channel) {
};

void TLC5947pin::_set(bool state) {
  tlc->setPWM(channel, state*intensity);
  tlc->write();
};

void TLC5947pin::print() {
	DEBUG("TLC5947 pin ");
	DEBUG(channel);
	DEBUG(" intensity ");
	DEBUG(intensity);
	DEBUG(" LN Address: ");
	DEBUGLN(_address);
}