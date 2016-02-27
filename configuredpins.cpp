#include <configuredpins.h>
#include <Arduino.h>

#ifndef DEBUG
#define DEBUG(x)
#endif

    InputPin::InputPin(uint8_t confpin, uint8_t pin, uint16_t address) : ConfiguredPin(confpin, pin, address){pinMode(_pin, INPUT_PULLUP); state = 0; laststate = 1;};
    void InputPin::print() {DEBUG("Input pin ");DEBUG(_pin);DEBUG("\n");};
    bool InputPin::update() {
      state = digitalRead(_pin);
      //DEBUG(state);
      if (state != laststate) {
        reportSensor(_address, state);
        DEBUG("State pin ");
        DEBUG(_pin);
        DEBUG(" changed to ");
        DEBUG(state);
        laststate = state;
		return true;
      }
	  return false;
    }

ServoSwitch::ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address) : ConfiguredPin(confpin, pin, address){};
ServoSwitch::ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address, uint16_t pos1, uint16_t pos2, uint16_t speed, uint8_t powerpin) :ConfiguredPin(confpin, pin, address) {
  _straight = pos1;
  _turnout = pos2;
  _speed = speed;
  _powerpin = powerpin;

  _currentpos = _straight;
  _targetpos  = _currentpos;
  _currentspeed = 0;

  _currentdelay = 0;
  _opstate = START;
  };

void ServoSwitch::changepin(uint8_t pin) {_servo.detach(); _pin = pin; _servo.attach(pin);};

void ServoSwitch::set(bool dir, bool state) {
  digitalWrite(_powerpin, HIGH);
  if (dir){
	  _targetpos = _turnout;
  } else {
	  _targetpos = _straight;
  }
  if (_targetpos < _currentpos) {
  	 _currentspeed = -1* _speed;
 } else {
	 _currentspeed = _speed;
 }
  reportSwitch(_address);
  _state = dir;
  _opstate = MOVE;
};

void ServoSwitch::toggle() {
	set(!_state, 0);
};

bool ServoSwitch::update () {
  if (_opstate == STOP ) {
    return false;
  }

  if (abs(_currentpos - _targetpos) < abs(_currentspeed)){
    _opstate = STOP;
    return false;
  }

	_currentdelay++;
  if (_opstate == MOVE){
	   if (_currentdelay > 250) {
		     //DEBUG("Moving to");
		     _currentpos = _currentpos + _currentspeed;
    	   _servo.writeMicroseconds(_currentpos);
		    //DEBUG(_currentpos);
		    //DEBUG("\n");
		    _currentdelay = 0;
        if (_currentspeed == 0 && (_targetpos == _currentpos)) {
          _currentspeed = 1;
        }
	   }
     return true;
  }
  if (_opstate == START) {
    if (_currentdelay > 250) {
      _servo.attach(_pin);
      DEBUG("Attached !\n");
      _opstate = STOP;
      return true;
    }
  }
  return false;
};

void ServoSwitch::print(){

  DEBUG("Arduino Pin: ");
  DEBUG(_pin);
  DEBUG(" Straight: ");
  DEBUG(_straight);
  DEBUG(" Turnout: ");
  DEBUG(_turnout);
  DEBUG(" Address: ");
  DEBUG(_address);
  DEBUG(" Speed: ");
  DEBUG(_speed);
  DEBUG("\n");
};

void ServoSwitch::print_state() {
  if (_state) {
    DEBUG("Straight");
  } else {
    DEBUG("Thrown");
  }
  DEBUG(_servo.read());
};

uint16_t ServoSwitch::get_state() {
	return _state;
}
void ServoSwitch::restore_state(uint16_t state){
	_state = state;
  digitalWrite(_powerpin, HIGH);
  if (_state) {
    _targetpos = _turnout;
  } else {
    _targetpos = _straight;
  }
  _currentpos = _targetpos;
  _currentspeed = 0;
  _opstate = START;
}
