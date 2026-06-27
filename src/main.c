#include <unistd.h>
#include <sndfile.h>
#include <stdbool.h>
#include "args.h"
#include "log.h"
#include "demodulate.h"

int main(int argc, char** argv)
{
    Args args;

    if (!parse_args(argc, argv, &args))
    {
        return 1;
    }

    switch (args.input_mode)
    {
    case INPUT_MODE_FILE:
        LOG_INFO("Input mode set to INPUT_MODE_FILE.");
        LOG_DEBUG("File path specified: %s", args.input.file_path);
        demodulate_file(args.input.file_path);
        break;

    case INPUT_MODE_RECORD:
        break;

    case INPUT_MODE_STREAM:
        break;

    default:
        break;
    }

    return 0;
}
