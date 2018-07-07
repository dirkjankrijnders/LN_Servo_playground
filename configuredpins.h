#ifndef CONFIGURED_PINS_H
#define CONFIGURED_PINS_H

#include "config.h"
#include <stdint.h>

#if PINSERVO == 1
#warning "USING SERVO"
  #include <Servo.h>
#endif


extern void reportSwitch(uint16_t address, uint16_t _state);
extern void reportSensor(uint16_t address, bool _state);
extern void reportSlot(uint16_t slot, uint16_t state);
extern void setSlot(uint16_t slot, uint16_t state);

class ConfiguredPin{
  public:
    ConfiguredPin(uint8_t confpin, uint8_t pin, uint16_t address) {_confpin = confpin; _pin = pin, _address = address;};
	virtual ~ConfiguredPin() {};
    virtual void print() {};
    virtual void toggle() {};
    virtual void set(bool port, bool state) {};
	virtual void _set(bool state) {};
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
    InputPin(uint8_t confpin, uint8_t pin, uint16_t address, bool report_inverse = 0, uint16_t secondary_address = 0);
	void set(bool force, bool state);
    void print();
    bool update();
    bool _state;
    bool _laststate;
	uint16_t _secondary_address;
	bool _report_inverse;
};

class OutputPin : public ConfiguredPin {
public:
    OutputPin(uint8_t confpin, uint8_t pin, uint16_t address, bool cumulative = 0, bool force_on = 0);
    void print();
    bool update();
    bool state;
	void set(bool port, bool state);
	void _set(bool state);
	void toggle();
private:
  bool _cumulative;
  bool _force_on;
  int8_t _accumulator;
};

class MagnetSwitch : public ConfiguredPin {
public:
  MagnetSwitch(uint8_t confpin, uint8_t pin1, uint16_t address, uint16_t pin2, uint16_t duration, uint16_t fbslot1, uint16_t fbslot2);
  ~MagnetSwitch();
  void print();
  void print_state();
  void set(bool dir, bool state);
  bool update ();
	uint16_t _pin2;
	uint16_t _duration;
	uint16_t _fbslot1;
	uint16_t _fbslot2;
	unsigned long _timer;
  bool _state;
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
#if PINSERVO
    Servo _servo;
#endif
	void reportSwitch();
	typedef enum {
		START = 0,
		MOVE,
		STOP
	} states;
  protected:
	uint16_t _currentpos;
	uint16_t _targetpos;
	int16_t _currentspeed;
	uint8_t _currentdelay;
  states _opstate;
};

class DualAction : public ConfiguredPin {
public:
	DualAction(uint8_t confpin, uint8_t pin, uint16_t address, uint16_t actionslot1, uint16_t actionslot2, uint16_t delay, uint16_t options);
    void print();
    bool update();
    bool state;
	void set(bool port, bool state);
	void toggle();
private:
	uint8_t _actionslot1;
	uint8_t _actionslot2;
	uint16_t _delay;
	uint32_t _lastmilli;
	typedef enum {
		IDLE = 0,
		FIRST,
		SECOND
	} movestates;
	movestates _state;
	bool _portstate;
	bool _direc;
};
#endif
