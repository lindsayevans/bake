#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

#include <iostream>
#include <unordered_map>

#include "main.hpp"
#include "arguments.hpp"
#include "util/string.hpp"

using namespace std;

#define ARG_PRINT_CONFIG 1000

map<string, string> populate_defines(struct program_arguments args, char *optarg)
{
    map<string, string> defines = parse_key_value_string(optarg, ',');
    for (auto &[key, value] : defines)
    {
        args.defines.insert_or_assign(key, value);
    }

    return args.defines;
}

struct program_arguments parse_arguments(int argc, char **argv)
{
    struct program_arguments arguments = {.exit_status = -1, .quiet = false, .config_file = "bake.json"};

    int c;

    while (1)
    {
        const char *short_options = "qvhc:D:";
        static struct option long_options[] = {
            {"version", no_argument, NULL, 'v'},
            {"help", no_argument, NULL, 'h'},
            {"quiet", no_argument, NULL, 'q'},
            {"config-file", required_argument, NULL, 'c'},
            {"D", required_argument, NULL, 'D'},
            {"print-config", no_argument, NULL, ARG_PRINT_CONFIG},
            {0, 0, 0, 0}};
        int option_index = 0;

        c = getopt_long(argc, argv, short_options, long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
        case 'v':
            print_version();
            arguments.exit_status = EXIT_SUCCESS;
            return arguments;
            break;

        case 'h':
            print_usage();
            arguments.exit_status = EXIT_SUCCESS;
            return arguments;
            break;

        case 'q':
            arguments.quiet = true;
            break;

        case 'c':
            arguments.config_file = optarg;
            break;

        case 'D':
            // Define a variable
            // Expects NAME=VALUE
            arguments.defines = populate_defines(arguments, optarg);
            break;

        case ARG_PRINT_CONFIG:
            arguments.print_config = true;
            break;

        case '?':
            break;

        default:
            abort();
        }
    }

    // Remaining arguments are targets to run
    if (optind < argc)
    {
        while (optind < argc)
        {
            arguments.targets.push_back(argv[optind++]);
        }
    }

    return arguments;
}

void print_version(void)
{
    printf("%s v%s\n", PROGRAM_NAME, PROGRAM_VERSION_STRING);
}

void print_usage(void)
{
    print_version();
    printf("\nUsage:\n");
    printf("%s [options] [target]\n\n", PROGRAM_NAME);
    printf("Options:\n");
    printf("--quiet, -q\tQuiet mode - just output the result\n");
    printf("--help, -h\tPrint out usage information\n");
    printf("--version, -v\tPrint version information\n");
    printf("--config-file, -c\tPath to donfig file (default: bake.json)\n");
    printf("--print-config\tPrint configuration\n");
    printf("-D\tPDefine a variable (e.g. -DFOO=bar)\n");
}
