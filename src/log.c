#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "args.h"
#include "log.h"

static LogLevel current_level = LOG_NONE;

void log_set_level(LogLevel level) {
    current_level = level;
}

void log_message(LogLevel level, const char *fmt, ...) {
    if (level > current_level) {
        return;
    }

    switch (level) {
    case LOG_ERROR:
        fprintf(stderr, BOLD PRP "[ERROR] " RESET);
        break;

    case LOG_INFO:
        fprintf(stderr, BOLD YEL "[INFO ] " RESET);
        break;

    case LOG_DEBUG:
        fprintf(stderr, BOLD GRN "[DEBUG] " RESET);
        break;
    default:
        fprintf(stderr, BOLD BLU "[UNKNOWN LOG LEVEL] " RESET);
        break;
    }

    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, fmt, args);

    va_end(args);

    fprintf(stderr, "\n");
}

void fatal_error(const char *fmt, ...) {
    fprintf(stderr, BOLD RED "[FATAL] " RESET);

    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, fmt, args);

    va_end(args);

    fprintf(stderr, "\n");

    exit(1);
}
