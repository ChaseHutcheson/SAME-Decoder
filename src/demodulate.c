#include <stdlib.h>
#include <sndfile.h>
#include <unistd.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include "log.h"

#define MARK_FREQ 2083.3f
#define SPACE_FREQ 1562.5f
#define BIT_LEN_MICROSECS 1920.0f
#define THRESHOLD 1000

typedef enum {
    SPACE,
    MARK,
    UNDEFINED
} Symbol;

float goertzel(const float *samples, const size_t count, const float sample_rate, const float target_freq) {
    float omega = 2.0f * M_PI * target_freq / sample_rate;
    float coeff = 2.0f * cosf(omega);

    float q0 = 0.0f;
    float q1 = 0.0f;
    float q2 = 0.0f;

    for (size_t i = 0; i < count; i++) {
        q0 = coeff * q1 - q2 + samples[i];
        q2 = q1;
        q1 = q0;
    }

    return q1 * q1 + q2 * q2 - coeff * q1 * q2;
}

void demodulate_file(const char* file_path)
{
    /*
     * -- AUDIO FILE PROCESSING --
     */
    SF_INFO sf_info;
    SNDFILE *sf_ptr = sf_open(file_path, SFM_READ, &sf_info);

    if (sf_ptr == NULL)
    {
        FATAL_ERROR("Failed to open audio file. libsndfile error message: %s", sf_strerror(NULL));
    }

    LOG_DEBUG("Frames: %ld", sf_info.frames);
    LOG_DEBUG("Sample Rate: %d", sf_info.samplerate);
    LOG_DEBUG("Channels: %d", sf_info.channels);
    LOG_DEBUG("Format: %#x", sf_info.format);
    LOG_DEBUG("Sections: %d", sf_info.sections);
    LOG_DEBUG("Seekable: %d", sf_info.seekable);

    int total_samples = sf_info.frames * sf_info.channels;

    float *samples = malloc(total_samples * sizeof(float));
    float *samples_mono = malloc(sf_info.frames * sizeof(float));

    if (samples == NULL || samples_mono == NULL)
    {
        FATAL_ERROR("Failure to allocate memory for samples.");
    }

    LOG_DEBUG("Samples at memory address: %p", (void *)samples);
    LOG_DEBUG("Mono-converted samples at memory address: %p", (void *)samples_mono);

    sf_count_t frames_read = sf_readf_float(sf_ptr, samples, sf_info.frames);

    if (frames_read == 0) {
        FATAL_ERROR("Failure to read samples from file.");
    }

    if (sf_info.channels > 1) {
        LOG_INFO("Audio file is not MONO, downmixing to MONO.");
        for (int current_frame = 0; current_frame < sf_info.frames; ++current_frame) {

            float mono_sample = 0.0f;

            for (int current_sample = 0; current_sample < sf_info.channels; ++current_sample) {
                int index = current_frame * sf_info.channels + current_sample;

                mono_sample += samples[index];
            }

            mono_sample /= sf_info.channels;

            samples_mono[current_frame] = mono_sample;
        }

        LOG_INFO("MONO Downmixing complete!");
    } else {
        LOG_INFO("Audio file is was already MONO, skipped downmixing.");
        memcpy(samples_mono, samples, sf_info.frames * sizeof(float));
    }

    free(samples);


    /*
     * -- DEMODULATION --
     */

    // Config
    float bits_per_second = (1.0 / (BIT_LEN_MICROSECS / 1000000.0));
    float samples_per_bit = sf_info.samplerate / bits_per_second;

    int buffer_size = (int)round(samples_per_bit);

    int last_transition = 0;

    // Sliding Window along bitstream
    for
    (
        int window_position = 0;
        window_position <= sf_info.frames - buffer_size;
        window_position++
    )
    {
        float mark_energy = goertzel(&samples_mono[window_position], buffer_size, sf_info.samplerate, MARK_FREQ);
        float space_energy = goertzel(&samples_mono[window_position], buffer_size, sf_info.samplerate, SPACE_FREQ);
    }

    free(samples_mono);
    sf_close(sf_ptr);
}
