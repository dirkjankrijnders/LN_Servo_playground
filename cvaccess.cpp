#include "cvaccess.h"
#include <avr/eeprom.h>
#include <Arduino.h>

#define cvsPerPin 7
const uint16_t cv2address(uint16_t cv){
	uint16_t address = cv;
	if (cv < 32) {
		return cv-1;
	} else {
		cv -= 32;
		uint8_t pin_conf = cv/cvsPerPin;
		
		address = 31+(pin_conf*sizeof(conf_t)) + 2*(cv-(pin_conf*cvsPerPin));
	}
	return address;
}

uint8_t bytesizeCV(uint16_t cv) {
	if (cv < 32) {
		return 1;
	}
	return 2;
}

#ifdef __AVR__
uint8_t read_cv8(decoder_conf_t* CV, int16_t cv) {
	uint8_t add = (uint16_t)&(CV->address)+cv2address(cv);
	return eeprom_read_byte((uint8_t*)(&(CV->address)+cv2address(cv)));
}

uint16_t read_cv16(decoder_conf_t* CV, int16_t cv) {
	return eeprom_read_word((uint16_t*)(&(CV->address)+(uint16_t)cv2address(cv)));//&(CV->address)+cv2address(cv));
}

void write_cv8(decoder_conf_t* CV, int16_t cv, uint8_t value) {
	eeprom_write_byte((uint8_t*)(&(CV->address)+cv2address(cv)), value);
}

void write_cv16(decoder_conf_t* CV, int16_t cv, uint16_t value) {
	eeprom_write_word((uint16_t*)(&(CV->address)+cv2address(cv)), value);
}
#else
uint8_t read_cv8(decoder_conf_t* CV, int16_t cv) {
	return *(uint8_t*)((uint8_t*)CV+cv2eeaddress(cv));
}

uint16_t read_cv16(decoder_conf_t* CV, int16_t cv) {
	return *(uint16_t*)((uint8_t*)CV+cv2eeaddress(cv));
}

void write_cv8(decoder_conf_t* CV, int16_t cv, uint8_t value) {
	*(uint8_t*)((uint8_t*)&(CV->address)+cv2eeaddress(cv)) = value;
}

void write_cv16(decoder_conf_t* CV, int16_t cv, uint16_t value) {
	*(uint16_t*)((uint8_t*)&(CV->address)+cv2eeaddress(cv)) = value;
}
#endif

uint16_t read_cv(decoder_conf_t* CV, int16_t cv) {
	if (bytesizeCV(cv) == 2) {
		return read_cv16(CV, cv);
	} else {
		return read_cv8(CV, cv);
	}
}

void write_cv(decoder_conf_t* CV, int16_t cv, uint16_t value) {
	if (bytesizeCV(cv) == 2) {
		write_cv16(CV, cv, value);
	} else {
		write_cv8(CV, cv, value);
	}
}
