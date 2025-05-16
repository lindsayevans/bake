
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include <jansson.h>

#include "util/string.hpp"
#include "config.hpp"
#include "arguments.hpp"

using namespace std;

struct config load_config(struct program_arguments args)
{
    struct config config = {.loaded = false};

    json_t *json;
    json_error_t error;

    json = json_load_file(args.config_file.c_str(), 0, &error);
    if (!json)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return config;
    }
    if (!json_is_object(json))
    {
        fprintf(stderr, "error: json is not an object\n");
        json_decref(json);
        return config;
    }

    config.loaded = true;

    const char *key;
    json_t *value;
    json_object_foreach(json, key, value)
    {
        char *k = strdup(key);
        if (strcmp(k, "name") == 0)
        {
            config.name = strdup(json_string_value(value));
        }
        if (strcmp(k, "version") == 0)
        {
            config.version = strdup(json_string_value(value));
        }
        if (strcmp(k, "description") == 0)
        {
            config.description = strdup(json_string_value(value));
        }
        if (strcmp(k, "variables") == 0)
        {
            config.variables = get_variables(config, value, args.defines);
        }
        if (strcmp(k, "targets") == 0)
        {
            config.targets = get_targets(config, value);
        }
    }

    json_decref(json);

    return config;
}

map<string, string> get_variables(struct config config, json_t *variables_json, map<string, string> defines)
{
    const char *key;
    json_t *value;
    json_object_foreach(variables_json, key, value)
    {
        config.variables.insert(std::pair{key, json_string_value(value)});
    }

    // TODO: should be making a second pass to interpolate variables after they've all been added
    for (auto &[key, value] : config.variables)
    {
        config.variables[key] = interpolate_variable(config, value);
    }

    for (auto &[key, value] : defines)
    {
        config.variables.insert_or_assign(key, interpolate_variable(config, value));
    }

    return config.variables;
}

string interpolate_variable(struct config config, string source)
{
    replace_all(source, "${:NAME}", config.name);
    replace_all(source, "${:VERSION}", config.version);
    replace_all(source, "${:DESCRIPTION}", config.description);

    for (auto &[key, value] : config.variables)
    {
        replace_all(source, "${" + key + "}", value);
    }

    return source;
}

string get_variable(struct config config, string key)
{
    return config.variables[key];
}

struct config_target get_target(struct config config, json_t *target_json)
{

    struct config_target target = {};
    const char *key;
    json_t *value;

    json_object_foreach(target_json, key, value)
    {

        char *k = strdup(key);
        if (strcmp(k, "default") == 0)
        {
            target.is_default = json_boolean_value(value) == 1;
        }

        if (strcmp(k, "in") == 0)
        {
            size_t index;
            json_t *v;
            json_array_foreach(value, index, v)
            {
                target.in.push_back(interpolate_variable(config, json_string_value(v)));
            }
        }

        if (strcmp(k, "transitive") == 0)
        {
            size_t index;
            json_t *v;
            json_array_foreach(value, index, v)
            {
                target.transitive.push_back(interpolate_variable(config, json_string_value(v)));
            }
        }

        if (strcmp(k, "cmd") == 0)
        {
            size_t index;
            json_t *v;
            json_array_foreach(value, index, v)
            {
                target.cmd.push_back(interpolate_variable(config, json_string_value(v)));
            }
        }

        if (strcmp(k, "cmdMulti") == 0)
        {
            target.cmd_mutiple = json_boolean_value(value) == 1;
        }
    }

    return target;
}

map<string, config_target> get_targets(struct config config, json_t *targets_json)
{
    const char *key;
    json_t *value;
    json_object_foreach(targets_json, key, value)
    {
        struct config_target target = get_target(config, value);
        config.targets.insert(std::pair{interpolate_variable(config, key), target});
    }

    return config.targets;
}

vector<string> get_default_targets(config config)
{
    vector<string> defaults = {};

    for (auto &[key, value] : config.targets)
    {
        if (value.is_default)
        {
            defaults.push_back(key);
        }
    }

    // No default targets, grab the first one
    // TODO: Not working for multiple
    if (defaults.size() == 0)
    {
        defaults.push_back(config.targets.begin()->first);
    }

    return defaults;
}

void print_config(struct config config)
{
    cout << "config:" << endl
         << " - name: " << config.name << endl
         << " - version: " << config.version << endl
         << " - description: " << config.description << endl
         << " - variables: " << endl;

    for (auto &[key, value] : config.variables)
    {
        cout << "   " << key << ": " << value << endl;
    }

    cout << " - targets: " << endl;
    for (auto &[target_name, target] : config.targets)
    {
        cout << "   " << target_name << (target.is_default ? " (default)" : "") << ": " << endl;
        if (target.in.size() > 0)
        {
            cout << "   - in: " << endl;
            for (string in : target.in)
            {
                cout << "       " << in << endl;
            }
        }
        if (target.transitive.size() > 0)
        {
            cout << "   - transitive: " << endl;
            for (string transitive : target.transitive)
            {
                cout << "       " << transitive << endl;
            }
        }
        if (target.cmd.size() > 0)
        {
            cout << "   - cmd: " << endl;
            for (string cmd : target.cmd)
            {
                cout << "       " << cmd << endl;
            }
        }
    }
}
