#ifndef CONFIGURED_PINS_H
#define CONFIGURED_PINS_H

#include <Servo.h>

extern void reportSwitch(uint16_t address, uint16_t _state);
extern void reportSensor(uint16_t address, bool _state);
extern void reportSlot(uint16_t slot, uint16_t state);

class ConfiguredPin{
  public:
    ConfiguredPin(uint8_t confpin, uint8_t pin, uint16_t address) {_confpin = confpin; _pin = pin, _address = address;};
	virtual ~ConfiguredPin() {};
    virtual void print() {};
    virtual void toggle() {};
    virtual void set(bool port, bool state) {};
	virtual void set_pin_cv(uint16_t PinCv, uint16_t value) {};
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
	void set(bool force, bool state);
    void print();
    bool update();
    bool _state;
    bool _laststate;
};

class ServoSwitch : public ConfiguredPin {
  public:
    ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address);
    ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address, uint16_t pos1, uint16_t pos2, uint16_t speed, uint8_t powerpin, uint16_t fbslot1, uint16_t fbslot2);
	~ServoSwitch();
    void changepin(uint8_t pin);
    void set(bool dir, bool state);
	void set_pin_cv(uint8_t PinCv, uint16_t value);
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
	uint16_t _fbslot1;
	uint16_t _fbslot2;
    bool _state;
    Servo _servo;
	void reportSwitch();
	typedef enum {
		START = 0,
		MOVE,
		STOP
	} states;
  private:
	uint16_t _currentpos;
	uint16_t _targetpos;
	int16_t _currentspeed;
	uint8_t _currentdelay;
  states _opstate;
};

#endif
