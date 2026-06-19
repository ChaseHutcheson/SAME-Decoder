#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sndfile.h>
#include <string.h>
#include "goertzel.h"

int main(int argc, char** argv) {
    // Check the user included a directory to a file
    if (argc == 1) {
        fprintf(stderr, "Please include a file path as the second argument.\n");
        return 1;
    }

    // Verify file exists
    if (access(argv[1], F_OK) == -1)
    {
        fprintf(stderr, "Provided file path isn't valid, or the file doesn't exist. Please check the path again.\n");
        return 1;
    }

    SF_INFO sf_info;
    SNDFILE *sf_ptr = sf_open(argv[1], SFM_READ, &sf_info);

    printf("Frames: %ld\n", sf_info.frames);
    printf("Sample Rate: %d\n", sf_info.samplerate);
    printf("Channels: %d\n", sf_info.channels);
    printf("Format: %#x\n", sf_info.format);
    printf("Sections: %d\n", sf_info.sections);
    printf("Seekable: %d\n", sf_info.seekable);

    int total_samples = sf_info.frames * sf_info.channels;

    float *samples = malloc(total_samples * sizeof(float));
    float *samples_mono = malloc(sf_info.frames * sizeof(float));

    if (samples == NULL || samples_mono == NULL)
    {
        fprintf(stderr, "Failure to allocate memory for samples.\n");
        return 1;
    }

    printf("Samples at memory address: %p\n", (void *)samples);
    printf("Mono-converted samples at memory address: %p\n", (void *)samples_mono);

    sf_count_t frames_read = sf_read_float(sf_ptr, samples, sf_info.frames);

    if (frames_read == 0) {
        fprintf(stderr, "Failure to read samples from file.\n");
        return 1;
    }

    // Downmix into mono audio if needed
    if (sf_info.channels > 1) {
        printf("File is not mono. Downmixing to mono...");

        for (int current_frame = 0; current_frame < sf_info.frames; ++current_frame) {

            float mono_sample = 0.0f;

            for (int current_sample = 0; current_sample < sf_info.channels; ++current_sample) {
                int index = current_frame * sf_info.channels + current_sample;

                mono_sample += samples[index];
            }

            mono_sample /= sf_info.channels;

            samples_mono[current_frame * sf_info.channels] = mono_sample;
        }

        printf("Downmixing to mono complete.");
    } else {
        memcpy(samples_mono, samples, total_samples);
    }

    

    // for (int i = 0; i < (sf_info.frames * sf_info.channels); ++i) {
    //     printf("Sample %d: %f\n", i, samples[i]);

    // }



    free(samples);

    return 0;
}