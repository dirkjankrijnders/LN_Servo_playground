#include <configuredpins.h>
#include <Arduino.h>
#include <LocoNet.h>
/*#ifndef DEBUG
#define DEBUG(x)
#endif
*/
    InputPin::InputPin(uint8_t confpin, uint8_t pin, uint16_t address) : ConfiguredPin(confpin, pin, address){pinMode(_pin, INPUT_PULLUP); state = 0; laststate = 1;};
    void InputPin::print() {DEBUG("Input pin ");DEBUG(_pin);DEBUG("\n");};
    void InputPin::update() {
      state = digitalRead(_pin);
      //DEBUG(state);
      if (state != laststate) {
        LocoNet.reportSensor(_address, state);
        DEBUG("State pin ");
        DEBUG(_pin);
        DEBUG(" changed to ");
        DEBUG(state);
        laststate = state;
      }
    }
  
ServoSwitch::ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address) : ConfiguredPin(confpin, pin, address){};
ServoSwitch::ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address, uint16_t pos1, uint16_t pos2, uint16_t speed, uint8_t powerpin) :ConfiguredPin(confpin, pin, address) { 
  _straight = pos1;
  _turnout = pos2;
  _speed = speed;
  _powerpin = powerpin
  _servo.attach(pin);
  };

void ServoSwitch::changepin(uint8_t pin) {_servo.detach(); _pin = pin; _servo.attach(pin);};

void ServoSwitch::set(bool dir, bool state) {
	digitalWrite(_powerpin, HIGH);
  if (dir){
    _servo.writeMicroseconds(_turnout);
  } else {
    _servo.writeMicroseconds(_straight);
  }
  LocoNet.reportSwitch(_address);
  _state = dir;
};

void ServoSwitch::toggle() {
	digitalWrite(_powerpin, HIGH);
  if (_state) {
    _servo.writeMicroseconds(_straight);
    _state = !_state;
  } else {
    _servo.writeMicroseconds(_turnout);
    _state = !_state;
  }
};

void ServoSwitch::update () {
	digitalWrite(_powerpin, LOW);
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
