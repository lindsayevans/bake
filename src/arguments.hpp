#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

struct program_arguments
{
    int8_t exit_status;
    bool quiet;
    bool print_config;
    bool print_tree;
    bool dry_run;
    string config_file;
    map<string, string> defines = {};
    vector<string> targets = {};
};

struct program_arguments parse_arguments(int argc, char **argv);

void print_version();
void print_usage();
