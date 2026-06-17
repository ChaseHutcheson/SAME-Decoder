#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "wav.h"

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

    wav_file_t* wav_file;

    if ((wav_file = parse_wav_file(argv[1])) == NULL) {
        fprintf(stderr, "Failed to parse WAVE file, exiting...\n");
        return 1;
    }

    print_wav_header(&(wav_file->header));

    free(wav_file);

    return 0;
}