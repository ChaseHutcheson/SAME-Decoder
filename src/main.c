#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sndfile.h>
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

    float *samples = malloc(sf_info.frames * sf_info.channels * sizeof(float));

    printf("Samples at memory address: %p\n", (void *)samples);

    sf_count_t frames_read = sf_read_float(sf_ptr, samples, sf_info.frames);

    // for (int i = 0; i < (sf_info.frames * sf_info.channels); ++i) {
    //     printf("Sample %d: %f\n", i, samples[i]);

    // }

    free(samples);

    return 0;
}