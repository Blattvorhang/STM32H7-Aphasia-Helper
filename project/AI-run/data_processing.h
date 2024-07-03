#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include "ai_platform.h"
void RGB565_to_RGB888(uint16_t rgb565, uint8_t *r, uint8_t *g, uint8_t *b);
void RGB888_to_RGB565(uint8_t r, uint8_t g, uint8_t b, uint16_t *rgb565);
void acquire_and_process_data(int8_t *data);
void post_process(const int8_t *data);

#endif /* DATA_PROCESSING_H */
