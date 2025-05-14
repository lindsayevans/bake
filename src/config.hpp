#pragma once

#include <map>
#include <string>

#include <jansson.h>

using namespace std;

struct config
{
    bool loaded;
    string name;
    string version;
    string description;
    map<string, string> variables;
};

struct config load_config(string path);
void print_config(struct config config);
map<string, string> get_variables(struct config config, json_t *value);
string interpolate_variable(struct config config, string value);
string get_variable(struct config config, string key);