#include "bus_configuredpins.h"

#define PULSELENGTH 1000/60/4096

TLC5947pin::TLC5947pin(Adafruit_TLC5947* _tlc, uint8_t confpin, uint8_t pin, uint16_t address, bool cumulative, uint8_t _channel, uint16_t _intensity) 
	: OutputPin(confpin, pin, address, cumulative), tlc(_tlc), intensity(_intensity), channel(_channel) {
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

PCA9685Servo::PCA9685Servo(Adafruit_PWMServoDriver* _pca, uint8_t confpin, uint8_t pin, uint16_t address, uint16_t pos1, uint16_t pos2, uint16_t speed, uint8_t powerpin, uint16_t fbslot1, uint16_t fbslot2) 
: ServoSwitch(confpin, pin, address, pos1, pos2, speed, powerpin, fbslot1, fbslot2), pca(_pca) {
	// Convert microseconds to pca ticks. i.e. position / PULSELENGTH
	_straight = pos1 / 4.069;
	_turnout = pos2 / 4.069;
};

bool PCA9685Servo::update () {
  if (_opstate == STOP ) {
    return false;
  }

  if (abs(_currentpos - _targetpos) < abs(_currentspeed)){
    _opstate = STOP;
		setSlot(_powerpin, 0);
		reportSwitch();
    return false;
  }

	_currentdelay++;
  if (_opstate == MOVE){
	   if (_currentdelay > 250) {
		   DEBUG("Moving to");
		     _currentpos = _currentpos + _currentspeed;
    	   //_servo.writeMicroseconds(_currentpos);
			 pca->setPWM(_pin, 0, _currentpos);
			 DEBUG(_currentpos);
			 DEBUG("\n");
		    _currentdelay = 0;
        if (_currentspeed == 0 && (_targetpos == _currentpos)) {
          _currentspeed = 1;
        }
	   }
     return true;
  }
  if (_opstate == START) {
    if (_currentdelay > 250) {
      //_servo.attach(_pin);
      DEBUG("Attached !\n");
      _opstate = MOVE;
      return true;
    }
  }
  return false;
};

void PCA9685Servo::print(){

  DEBUG("PCA9685 Pin: ");
  DEBUG(_pin);
  DEBUG(" Straight: ");
  DEBUG(_straight);
  DEBUG(" Turnout: ");
  DEBUG(_turnout);
  DEBUG(" Address: ");
  DEBUG(_address);
  DEBUG(" Speed: ");
  DEBUG(_speed);
  
  DEBUG(" Feedback slot 1: ");
  DEBUG(_fbslot1);
  DEBUG(" Feedback slot 2: ");
  DEBUG(_fbslot2);

  DEBUG(" Power slot: ");
  DEBUG(_powerpin);

  DEBUG("\n");
};
		