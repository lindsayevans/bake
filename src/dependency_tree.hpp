#pragma once

#include <map>
#include <vector>
#include <queue>
#include <string>

#include "config.hpp"

using namespace std;

struct dependency_tree
{
    string target_name;
    bool needs_rebuild;
    bool is_newer;
    bool cmd_mutiple;
    vector<string> in_files;
    vector<dependency_tree> dependencies;
};

vector<struct dependency_tree> build_dependency_trees(struct config config, vector<string> targets);
struct dependency_tree build_dependency_tree(struct config config, string target_name, struct dependency_tree *parent_tree);

void print_dependency_trees(vector<struct dependency_tree> trees);
void print_dependency_tree(struct dependency_tree tree, const string &prefix);

queue<string> build_command_queue(vector<struct dependency_tree> trees, struct config config);
void build_tree_command_queue(queue<string> *command_queue, vector<struct dependency_tree> trees, struct config config);
void print_command_queue(queue<string> command_queue);
