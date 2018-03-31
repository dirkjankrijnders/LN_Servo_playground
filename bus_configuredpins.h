#ifndef BUS_CONFIGUREDPINS_H
#define BUS_CONFIGUREDPINS_H

#include "configuredpins.h"
#include <Wire.h>

#include <Adafruit_PWMServoDriver.h>
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

class PCA9685Servo : public ServoSwitch {
	private:
		Adafruit_PWMServoDriver* pca;
	public:
		PCA9685Servo(Adafruit_PWMServoDriver* _pca, uint8_t confpin, uint8_t pin, uint16_t address, uint16_t pos1, uint16_t pos2, uint16_t speed, uint8_t powerpin, uint16_t fbslot1, uint16_t fbslot2);
		bool update();
		void print();
		void set_pin_cv(uint8_t PinCv, uint16_t value);
	};
	
	
#endif //BUS_CONFIGUREDPINS_H