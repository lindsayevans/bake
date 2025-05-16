
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#include "dependency_tree.hpp"
#include "config.hpp"
#include "util/string.hpp"

using namespace std;

vector<struct dependency_tree> build_dependency_trees(struct config config, vector<string> targets)
{
    vector<struct dependency_tree> trees = {};
    struct dependency_tree root_tree = {};
    for (const string &target : targets)
    {
        trees.push_back(build_dependency_tree(config, target, &root_tree));
    }

    return trees;
}

bool is_newer(string a, string b)
{
    filesystem::file_time_type a_time = filesystem::last_write_time(a);
    filesystem::file_time_type b_time = filesystem::last_write_time(b);

    return a_time > b_time;
}

struct dependency_tree build_dependency_tree(struct config config, string target_name, struct dependency_tree *parent_tree)
{
    struct dependency_tree target_tree = {.target_name = target_name, .needs_rebuild = false};

    if (!filesystem::exists(target_name))
    {
        target_tree.needs_rebuild = true;
        parent_tree->needs_rebuild = true;
    }
    else if (filesystem::exists(parent_tree->target_name))
    {
        target_tree.is_newer = is_newer(target_name, parent_tree->target_name);
    }

    for (auto &[name, target] : config.targets)
    {
        if (name == target_name)
        {
            target_tree.cmd_mutiple = target.cmd_mutiple;
            for (const string &in : target.in)
            {
                target_tree.in_files.push_back(in);
                target_tree.dependencies.push_back(build_dependency_tree(config, in, &target_tree));
            }
            for (const string &transitive : target.transitive)
            {
                target_tree.dependencies.push_back(build_dependency_tree(config, transitive, &target_tree));
            }
        }
    }

    return target_tree;
}

void print_dependency_trees(vector<struct dependency_tree> trees)
{
    for (const struct dependency_tree &tree : trees)
    {
        cout << endl
             << (tree.needs_rebuild ? RED : GREEN) << tree.target_name << RESET << endl;
        print_dependency_tree(tree, "");
    }
}

void print_dependency_tree(struct dependency_tree tree, const string &prefix)
{
    if (tree.dependencies.empty())
    {
        return;
    }

    size_t num_deps = tree.dependencies.size();
    cout << prefix;
    cout << (num_deps > 1 ? "├── " : "");

    for (size_t i = 0; i < num_deps; ++i)
    {
        struct dependency_tree c = tree.dependencies[i];
        if (i < num_deps - 1)
        {
            if (i > 0)
            {
                cout << prefix << "├── ";
            }
            bool printStrand = num_deps > 1 && !c.dependencies.empty();
            string newPrefix = prefix + (printStrand ? "│   " : "   ");
            cout << (c.needs_rebuild ? RED : (c.is_newer ? BLUE : GREEN)) << c.target_name << RESET << endl;
            print_dependency_tree(c, newPrefix);
        }
        else
        {
            cout << (num_deps > 1 ? prefix : "") << "└── ";
            cout << (c.needs_rebuild ? RED : (c.is_newer ? BLUE : GREEN)) << c.target_name << RESET << endl;
            print_dependency_tree(c, prefix + "    ");
        }
    }
}

string interpolate_command(string cmd, struct dependency_tree tree, string in)
{
    replace_all(cmd, "${:TARGET}", tree.target_name);
    replace_all(cmd, "${:IN}", in);
    return cmd;
}

void build_tree_command_queue(queue<string> *command_queue, vector<struct dependency_tree> trees, struct config config)
{

    for (const struct dependency_tree &tree : trees)
    {
        build_tree_command_queue(command_queue, tree.dependencies, config);

        if (tree.needs_rebuild)
        {
            for (const string &cmd : config.targets[tree.target_name].cmd)
            {
                if (tree.cmd_mutiple)
                {
                    for (const string &in : tree.in_files)
                    {
                        command_queue->push(interpolate_command(cmd, tree, in));
                    }
                }
                else
                {
                    string in = string_join(tree.in_files, " ");
                    command_queue->push(interpolate_command(cmd, tree, in));
                }
            }
        }
    }
}

queue<string> build_command_queue(vector<struct dependency_tree> trees, struct config config)
{
    queue<string> command_queue;
    build_tree_command_queue(&command_queue, trees, config);
    return command_queue;
}

void print_command_queue(queue<string> command_queue)
{
    while (!command_queue.empty())
    {
        std::cout << command_queue.front() << endl;
        command_queue.pop();
    }
}
