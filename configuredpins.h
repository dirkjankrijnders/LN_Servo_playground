#ifndef CONFIGURED_PINS_H
#define CONFIGURED_PINS_H

#include <Servo.h>

class ConfiguredPin{
  public:
    ConfiguredPin(uint8_t confpin, uint8_t pin, uint16_t address) {_confpin = confpin; _pin = pin, _address = address;};
    virtual void print() {};
    virtual void toggle() {};
    virtual void set(bool port, bool state) {};
    virtual void print_state() {};
    virtual bool update() {return false;};
	virtual void restore_state(uint16_t state) {};
	virtual uint16_t get_state() {return 0;};
  public:
    uint8_t _pin;
    uint16_t _address;
  private:
    uint8_t _confpin;
};

class InputPin : public ConfiguredPin {
  public:
    InputPin(uint8_t confpin, uint8_t pin, uint16_t address);
    void print();
    bool update();
    bool state;
    bool laststate;
};
  
class ServoSwitch : public ConfiguredPin {
  public:
    ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address);
    ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address, uint16_t pos1, uint16_t pos2, uint16_t speed, uint8_t powerpin);
    void changepin(uint8_t pin);
    void set(bool dir, bool state);
    void toggle();
    bool update ();
    void print();
    void print_state();
	void restore_state(uint16_t state);
	uint16_t get_state();
	uint16_t _turnout;
    uint16_t _straight;
    uint16_t _speed;
	uint8_t _powerpin;
    bool _state;
    Servo _servo;
  private:
	uint16_t _currentpos;
	uint16_t _targetpos;
	int16_t _currentspeed; 
	uint8_t _currentdelay;
};

#endif
