#pragma once
#include "args.h"

// ANSI Indicators
#define RESET   "\x1b[0m"
#define BOLD    "\x1b[1m"

// ANSI Color Codes
#define RED "\x1b[31m"
#define GRN "\x1b[32m"
#define YEL "\x1b[33m"
#define BLU "\x1b[34m"
#define PRP "\x1b[35m"

void log_message(LogLevel level, const char *fmt, ...);
void log_set_level(LogLevel level);
void fatal_error(const char *fmt, ...);

#define LOG_ERROR(...) log_message(LOG_ERROR, __VA_ARGS__)
#define LOG_INFO(...)  log_message(LOG_INFO,  __VA_ARGS__)
#define LOG_DEBUG(...) log_message(LOG_DEBUG, __VA_ARGS__)
#define FATAL_ERROR(...) fatal_error(__VA_ARGS__)
