#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

using namespace std;

void replace_all(string &str, const string &from, const string &to);
map<string, string> parse_key_value_string(const string &input, char delimiter);
string string_join(const vector<string> &strs, const string delim);
