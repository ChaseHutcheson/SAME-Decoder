#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#define PI 3.14159265358979323846f

float goertzel(const float *samples, size_t count, float sample_rate, float target_freq) {
    float omega = 2.0f * PI * target_freq / sample_rate;
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