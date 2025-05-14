
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <string>
#include <iostream>
#include <map>

#include <jansson.h>

#include "util/string.hpp"
#include "config.hpp"

using namespace std;

struct config load_config(string path)
{
    struct config config = {.loaded = false};

    json_t *json;
    json_error_t error;

    json = json_load_file(path.c_str(), 0, &error);
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
            config.variables = get_variables(config, value);
        }
    }

    json_decref(json);

    return config;
}

map<string, string> get_variables(struct config config, json_t *variables_json)
{
    const char *key;
    json_t *value;
    json_object_foreach(variables_json, key, value)
    {
        config.variables.insert(std::pair{key, json_string_value(value)});
    }

    for (auto &[key, value] : config.variables)
    {
        config.variables[key] = interpolate_variable(config, value);
    }

    return config.variables;
}

string interpolate_variable(struct config config, string source)
{
    replace_all(source, "${:name}", config.name);
    replace_all(source, "${:version}", config.version);
    replace_all(source, "${:description}", config.description);

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
}
