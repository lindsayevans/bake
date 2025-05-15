#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <iostream>
#include <vector>

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
        cerr << "Error loading config file " << args.config_file << endl;
        return EXIT_FAILURE;
    }
    if (args.print_config)
    {
        print_config(config);
    }

    vector<string> targets = merge_targets(get_default_targets(config), args.targets);
    if (targets.size() == 0)
    {
        cerr << "No targets" << endl;
        return EXIT_FAILURE;
    }
    else
    {
        cout << "Targets:" << endl;
        for (const string &target : targets)
        {
            cout << target << endl;
        }
    }

    return EXIT_SUCCESS;
}

vector<string> merge_targets(vector<string> default_targets, vector<string> provided_targets)
{

    vector<string> targets = provided_targets.size() > 0 ? provided_targets : default_targets;

    for (vector<string>::iterator target = targets.begin(); target != targets.end();)
    {
        // Replaces ":default" with the default targets
        if (*target == ":default")
        {
            targets.erase(target);
            targets.insert(target, default_targets.begin(), default_targets.end());
        }
        else
        {
            ++target;
        }
    }

    return targets;
}
