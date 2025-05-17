#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <iostream>
#include <vector>
#include <queue>

#include "main.hpp"
#include "arguments.hpp"
#include "config.hpp"
#include "dependency_tree.hpp"
#include "execute.hpp"

int main(int argc, char **argv)
{
    struct program_arguments args = parse_arguments(argc, argv);
    if (args.exit_status > -1)
    {
        return args.exit_status;
    }

    struct config config = load_config(args);
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

    vector<struct dependency_tree> dependency_trees = build_dependency_trees(config, targets);
    if (args.print_tree)
    {
        cout << "# Dependency tree:" << endl;
        print_dependency_trees(dependency_trees);
        cout << endl;
    }

    // TODO: command queue should be grouped by target, so we can show progress, lookup vars etc. - also potentially run commands in parallel
    queue<string> command_queue = build_command_queue(dependency_trees, config);
    if (args.dry_run)
    {
        cout << "# Dry run - commands that would execute:" << endl;
        print_command_queue(command_queue);
    }

    if (!args.quiet && !args.dry_run)
    {
        cout << endl
             << "Baking targets: ";
        for (const string &target : targets)
        {
            cout << target;
            if (target != targets.back())
            {
                cout << ", ";
            }
        }
        cout << endl
             << endl;
    }

    if (!args.dry_run)
    {
        bool result = execute_command_queue(command_queue, config, args);

        if (!result)
        {
            return EXIT_FAILURE;
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
