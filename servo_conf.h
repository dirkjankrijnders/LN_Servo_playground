#ifndef LNDECODER_H
#define LNDECODER_H

#include <inttypes.h>

typedef struct pin_conf {
	uint8_t func;
} __attribute__((packed)) pin_conf_t;

typedef struct servo_conf {
	uint16_t arduinopin;
	uint16_t address;
	uint16_t pos1;
	uint16_t pos2;
	uint16_t speed;
	uint16_t reserved1;
	uint16_t reserved2;
} __attribute__((packed)) servo_conf_t;

typedef struct led_conf {
	uint16_t arduinopin;
	uint16_t address;
	uint16_t value1;
	uint16_t value2;
	uint16_t function;
} __attribute__((packed)) led_conf_t;

typedef struct input_conf {
	uint16_t arduinopin;
	uint16_t address;
	uint16_t func;
} __attribute__((packed)) input_conf_t;

typedef union conf {
	servo_conf_t servo;
	led_conf_t led;
	input_conf_t input;
} __attribute__((packed)) conf_t;


#endif
