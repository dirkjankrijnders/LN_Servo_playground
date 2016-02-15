#ifndef EEPROM_H
#define EEPROM_H
#include "decoder_conf.h"
const uint16_t cv2address(uint16_t cv);
uint8_t bytesizeCV(uint16_t cv);
uint8_t read_cv8(decoder_conf_t* CV, int16_t cv);
uint16_t read_cv16(decoder_conf_t* CV, int16_t cv);
uint16_t read_cv(decoder_conf_t* CV, int16_t cv);
void write_cv8(decoder_conf_t* CV, int16_t cv, uint8_t value);
void write_cv16(decoder_conf_t* CV, int16_t cv, uint16_t value);
void write_cv(decoder_conf_t* CV, int16_t cv, uint16_t value);
#endif