#pragma once
#include <stdlib.h>
#include <stddef.h>

/*
 * Run the Goertzel algorithm on a number of PCM samples.
 */
float goertzel(const float *samples, const size_t count, const float sample_rate, const float target_freq);

/*
 * Demodulate a SAME Header from an audio file.
 */
void demodulate_file(const char* file_path);

/*
 * Demodulate a SAME Header from an audio device.
 */
void demodulate_device(const char* file_path);
