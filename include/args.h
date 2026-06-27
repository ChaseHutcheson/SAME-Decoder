#pragma once

#include <stdbool.h>

typedef enum {
    INPUT_MODE_UNDEFINED,
    INPUT_MODE_FILE,
    INPUT_MODE_RECORD,
    INPUT_MODE_STREAM
} InputMode;

typedef enum {
    LOG_NONE,
    LOG_ERROR,
    LOG_INFO,
    LOG_DEBUG
} LogLevel;

typedef struct {
    InputMode input_mode;
    union {
        const char *file_path;
        const char *device;
    } input;
    LogLevel log_level;
} Args;

bool parse_args(int argc, char** argv, Args* args);