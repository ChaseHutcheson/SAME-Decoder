#include "args.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

/*
    Parse arguments into Args variable
*/
bool parse_args(int argc, char** argv, Args* args) {
    for (int i = 1; i < argc; i++) {
        // HELP
        if (strcmp("--help", argv[i]) == 0)
        {
            printf(
                "This would be where the help would be, but I don't feel like writing it yet. Figure it out yourself.\n"
            );
            exit(EXIT_SUCCESS);
        }
        // VERSION
        else if (strcmp("--version", argv[i]) == 0)
        {
            printf("SAME-Decoder Version 0.0.1 <This is hardcoded btw - fix later>\n");
            exit(EXIT_SUCCESS);
        }
        // FILE MODE
        else if (strcmp("-f", argv[i]) == 0)
        {
            if (access(argv[++i], F_OK) == -1) {
                fprintf(stderr, "Error: Provided file path either doesn't exist or is incorrect. Please check path and try again.\n");
                return false;
            } else {
                args->input_mode = INPUT_MODE_FILE;
                args->input.file_path = argv[i];
            }
        }
        // DEVICE MODE
        else if (strcmp("-d", argv[i]) == 0)
        {
            if (access(argv[++i], F_OK) == -1) {
                fprintf(stderr, "Error: Provided device ID either doesn't exist or is incorrect. Please check ID and try again.\n");
                return false;
            } else {
                args->input_mode = INPUT_MODE_FILE;
                args->input.file_path = argv[i];
            }
        }
        // VERBOSE
        else if (strcmp("-v", argv[i]) == 0)
        {
            log_set_level(LOG_DEBUG);
        }
        else if (strcmp("-vi", argv[i]) == 0)
        {
            log_set_level(LOG_INFO);
        }
        else if (strcmp("-ve", argv[i]) == 0)
        {
            log_set_level(LOG_ERROR);
        }
        else
        {
            fprintf(stderr, "Error: Unknown Argument (%s).\n", argv[i]);
            return false;
        }
    }

    return true;
}
