#pragma once

#define MAX_PERCENTAGE_SIZE 256
#define MAX_VALUE_SIZE 256

struct ProgramArguments
{
    int8_t exitStatus;
    bool quiet;
    char *percentage;
    char *value;
};

struct ProgramArguments parseArguments(int argc, char **argv);

void printVersion();
void printUsage();
