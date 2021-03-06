#ifndef WAVELIB
#define WAVELIB

#include <stddef.h>
#include <stdint.h>

typedef struct wave {
    const char *filename;
    uint8_t *data;
} Wave;

Wave *wave_load(const char* filename);
void wave_destroy(Wave *wave);
int wave_get_bits_per_sample(Wave* wave);
int wave_get_number_of_channels(Wave *wave);
int wave_get_sample_rate(Wave *wave);
size_t wave_get_samples(Wave *wave, size_t frame_index, uint8_t *buffer, size_t frame_count);

#endif