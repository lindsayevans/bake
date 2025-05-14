#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

#include "main.h"
#include "arguments.h"

struct ProgramArguments parseArguments(int argc, char **argv)
{
    struct ProgramArguments arguments = {.exitStatus = -1, .quiet = false};

    int c;

    while (1)
    {
        static struct option long_options[] = {
            {"version", no_argument, NULL, 'v'},
            {"help", no_argument, NULL, 'h'},
            {"quiet", no_argument, NULL, 'q'},
            {0, 0, 0, 0}};
        int option_index = 0;

        c = getopt_long(argc, argv, "qvh", long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
        case 'v':
            printVersion();
            arguments.exitStatus = EXIT_SUCCESS;
            return arguments;
            break;

        case 'h':
            printUsage();
            arguments.exitStatus = EXIT_SUCCESS;
            return arguments;
            break;

        case 'q':
            arguments.quiet = true;
            break;

        case '?':
            break;

        default:
            abort();
        }
    }

    int pos = 1;
    if (optind < argc)
    {
        while (optind < argc)
        {
            if (pos == 2)
            {
                arguments.value = argv[optind];
                pos++;
            }

            if (pos == 1)
            {
                arguments.percentage = argv[optind];
                pos++;
            }

            optind++;
        }
    }

    // Prompt user for positional arguments if not supplied
    char percentageString[MAX_PERCENTAGE_SIZE] = "0";
    if (arguments.percentage == NULL)
    {
        printf(" %%: ");
        if (fgets(percentageString, MAX_PERCENTAGE_SIZE, stdin))
        {
            percentageString[strcspn(percentageString, "\n")] = 0;
            arguments.percentage = percentageString;
        }
    }

    char valueString[MAX_VALUE_SIZE] = "0";
    if (arguments.value == NULL)
    {
        printf("of: ");
        if (fgets(valueString, MAX_VALUE_SIZE, stdin))
        {
            valueString[strcspn(valueString, "\n")] = 0;
            arguments.value = valueString;
        }
    }

    return arguments;
}

void printVersion(void)
{
    printf("%s v%s\n", PROGRAM_NAME, PROGRAM_VERSION_STRING);
}

void printUsage(void)
{
    printVersion();
    printf("\nUsage:\n");
    printf("%s [options] [target]\n\n", PROGRAM_NAME);
    printf("Options:\n");
    printf("--quiet, -q\tQuiet mode - just output the result\n");
    printf("--help, -h\tPrint out usage information\n");
    printf("--version, -v\tPrint version information\n");
}
