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
    virtual void update() {};
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
    void update();
    bool state;
    bool laststate;
};
  
class ServoSwitch : public ConfiguredPin {
  public:
    ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address);
    ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address, uint16_t pos1, uint16_t pos2, uint16_t speed);
    void changepin(uint8_t pin);
    void set(bool dir, bool state);
    void toggle();
    void update ();
    void print();
    void print_state();
    uint16_t _turnout;
    uint16_t _straight;
    uint16_t _speed;
	uint8_t _powerpin;
    bool _state;
    Servo _servo;
  private:
};

#endif