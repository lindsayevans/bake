#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <iostream>

#include "main.hpp"
#include "arguments.hpp"
#include "config.hpp"

int main(int argc, char **argv)
{
    struct program_arguments args = parse_arguments(argc, argv);
    if (args.exit_status > -1)
    {
        return args.exit_status;
    }

    struct config config = load_config(args.config_file);
    if (!config.loaded)
    {
        // printf("Error loading config file %s\n", args.config_file);
        cerr << "Error loading config file " << args.config_file << endl;
        return EXIT_FAILURE;
    }
    print_config(config);

    return EXIT_SUCCESS;
}