#pragma once
#include <string>

struct program_arguments
{
    int8_t exit_status;
    bool quiet;
    bool print_config;
    std::string config_file;
};

struct program_arguments parse_arguments(int argc, char **argv);

void print_version();
void print_usage();
