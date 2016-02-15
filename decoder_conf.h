#ifndef DECODER_CONF_H_
#define DECODER_CONF_H_

#include <inttypes.h>

#include "servo_conf.h"

#define MAX_PINS 16
#define PIN_CONF_INPUT 0

typedef struct decoder_conf {
	uint8_t address;
	uint8_t cv2_5[4];
	uint8_t pins_conf;
	uint8_t manu_cv;
	uint8_t ver_cv;
	pin_conf_t pin_conf[MAX_PINS];
	uint8_t cv9_32[23-MAX_PINS];
	conf_t conf[MAX_PINS];
} __attribute__((packed)) decoder_conf_t;

#endif