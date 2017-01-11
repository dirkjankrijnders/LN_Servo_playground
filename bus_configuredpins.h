#include "configuredpins.h"
#include "config.h"

#include "Adafruit_TLC5947.h"

class TLC5947pin : public OutputPin {
private:
  Adafruit_TLC5947* tlc;
  uint16_t intensity;
  uint8_t channel;
public:
  TLC5947pin(Adafruit_TLC5947* _tlc, uint8_t confpin, uint8_t pin, uint16_t address, bool cumulative = 0, uint8_t _channel = 0, uint16_t _intensity = 1000);
  void _set(bool state);
  void print();
};
