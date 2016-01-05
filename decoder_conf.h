#ifndef DECODER_CONF_H_
#define DECODER_CONF_H_

#include <inttypes.h>

#include "servo_conf.h"

#define MAX_PINS 16

typedef struct decoder_conf {
	uint8_t address;
	uint8_t cv2_6[5];
	uint8_t manu_cv;
	uint8_t ver_cv;
	uint8_t cv9_32[23];
	conf_t conf[MAX_PINS];
} __attribute__((packed)) decoder_conf_t;

#endif