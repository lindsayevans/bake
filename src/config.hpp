#pragma once

#include <map>
#include <vector>
#include <string>

#include <jansson.h>

using namespace std;

struct config_target
{
    bool is_default;
    vector<string> in;
    vector<string> transitive;
    vector<string> cmd;
    map<string, string> variables;
};

struct config
{
    bool loaded;
    string name;
    string version;
    string description;
    map<string, string> variables;
    map<string, config_target> targets;
};

struct config load_config(string path);
void print_config(struct config config);
map<string, string> get_variables(struct config config, json_t *variables_json);
string interpolate_variable(struct config config, string value);
string get_variable(struct config config, string key);
map<string, config_target> get_targets(struct config config, json_t *targets_json);