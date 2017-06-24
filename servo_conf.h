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
	uint16_t state;
	uint16_t fbslot1;
	uint16_t fbslot2;
	uint16_t pwrslot;
} __attribute__((packed)) servo_conf_t;

typedef struct led_conf {
	uint16_t arduinopin;
	uint16_t address;
	uint16_t value1;
	uint16_t value2;
	uint16_t function;
	uint16_t reserved1;
	uint16_t state;
} __attribute__((packed)) led_conf_t;

typedef struct input_conf {
	uint16_t arduinopin;
	uint16_t address;
	uint16_t options;
	uint16_t secadd;
} __attribute__((packed)) input_conf_t;

typedef struct output_conf {
	uint16_t arduinopin;
	uint16_t address;
	uint16_t options;
	uint16_t fbslot1;
	uint16_t fbslot2;
}  __attribute__((packed)) output_conf_t;

typedef struct dualaction_conf {
	uint16_t arduinopin;
	uint16_t address;
	uint16_t actionslot1;
	uint16_t actionslot2;
	uint16_t delay;
	uint16_t options;	
	uint16_t state;
}  __attribute__((packed)) dualaction_conf_t;

typedef union conf {
	servo_conf_t servo;
	led_conf_t led;
	input_conf_t input;
	output_conf_t output;
	dualaction_conf_t dualaction;
} __attribute__((packed)) conf_t;


#endif
