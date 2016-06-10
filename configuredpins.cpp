#include <configuredpins.h>
#include <Arduino.h>

void ServoSwitch::reportSwitch(){
#ifdef DS54
	uint16_t address = confslot->address;
	byte AddrH = ( (--address >> 7) & 0x0F ) | OPC_SW_REP_INPUTS  ;
	byte AddrL = ( address) & 0x7F ;

	if ( state == 0 )
		AddrH |= OPC_SW_REP_SW  ;
	if ( state == 1 )
		AddrH |= OPC_SW_REP_HI  ;
	if ( state == 2 )
		AddrH |= OPC_SW_REP_HI | OPC_SW_REP_SW ;
	
	//LocoNet.send(OPC_SW_REP, AddrL, AddrH );
#else
	if ( _opstate == MOVE ) {
		reportSlot(_fbslot1, 0);
		reportSlot(_fbslot2, 0);
		//LocoNet.reportSensor(Address, 0);
		//LocoNet.reportSensor(Address+10, 0);
	} else if( _state == 0 ) {
		reportSlot(_fbslot1, 1);
		reportSlot(_fbslot2, 0);
		//LocoNet.reportSensor(Address, 1);
		//LocoNet.reportSensor(Address+10, 0);		
	} else if( _state == 1 ) {
		reportSlot(_fbslot1, 0);
		reportSlot(_fbslot2, 1);
		//LocoNet.reportSensor(Address, 0);
		//LocoNet.reportSensor(Address+10, 1);
	}
#endif
}


#ifndef DEBUG
#define DEBUG(x) Serial.print(x)
#endif

    InputPin::InputPin(uint8_t confpin, uint8_t pin, uint16_t address) : ConfiguredPin(confpin, pin, address){
		if (_pin > 15)
			_pin = 15;
		pinMode(_pin, INPUT_PULLUP);
		_state = 0;
		_laststate = 1;
	};
    void InputPin::print() {DEBUG("Input pin ");DEBUG(_pin);DEBUG("\n");};
    bool InputPin::update() {
		if (_pin > 0)
		  _state = digitalRead(_pin);
      //DEBUG(state);
      if (_state != _laststate) {
        reportSensor(_address, _state);
        DEBUG("State pin ");
        DEBUG(_pin);
        DEBUG(" changed to ");
        DEBUG(_state);
        _laststate = _state;
		return true;
      }
	  return false;
    }
	void InputPin::set(bool force, bool state) {
		if (force) {
			_laststate = !state;
		}
		_state = state;
	}

ServoSwitch::ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address) : ConfiguredPin(confpin, pin, address){
	_straight = 1500;
	_turnout = 1500;
	_speed = 2;
	_powerpin = 0;
	_currentpos = 1500;
	_targetpos = 1500;
	_fbslot1 = 0;
	_fbslot2 = 0;
	_currentspeed = 0;
	_currentdelay = 0;
	_opstate = START;
};
ServoSwitch::ServoSwitch(uint8_t confpin, uint8_t pin, uint16_t address, uint16_t pos1, uint16_t pos2, uint16_t speed, uint8_t powerpin, uint16_t fbslot1, uint16_t fbslot2) :ConfiguredPin(confpin, pin, address) {
  _straight = pos1;
  _turnout = pos2;
  _speed = speed;
  _powerpin = powerpin;

  _currentpos = _straight;
  _targetpos  = _currentpos;
  
  _fbslot1 = fbslot1;
  _fbslot2 = fbslot2;
  
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
  reportSwitch();
  _state = dir;
  _opstate = MOVE;
};

void ServoSwitch::set_pin_cv(uint8_t PinCv, uint16_t value) {
	switch (PinCv) {
		case 0: // Arduino Pin
			_pin = value;
			break;
		case 1: // Address
			_address = value;
			break;
		case 2: // Pos 1
			_straight = value;
			set(0, 0);
			break;
		case 3: // Pos 2
			_turnout = value;
			set(1, 0);
			break;
		case 4: // Speed
			_speed = value;
			break;
	}
	
}

void ServoSwitch::toggle() {
	set(!_state, 0);
};

bool ServoSwitch::update () {
  if (_opstate == STOP ) {
    return false;
  }

  if (abs(_currentpos - _targetpos) < abs(_currentspeed)){
    _opstate = STOP;
	reportSwitch();
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
      _opstate = MOVE;
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
  
  DEBUG(" Feedback slot 1: ");
  DEBUG(_fbslot1);
  DEBUG(" Feedback slot 2: ");
  DEBUG(_fbslot2);
  
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

ServoSwitch::~ServoSwitch() {
	DEBUG("Destructor!");
	_servo.detach();
}