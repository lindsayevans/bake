#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "arguments.h"

int main(int argc, char **argv)
{
    struct ProgramArguments args = parseArguments(argc, argv);
    if (args.exitStatus > -1)
    {
        return args.exitStatus;
    }

    return EXIT_SUCCESS;
}