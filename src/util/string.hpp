#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

void replace_all(string &str, const string &from, const string &to);
map<string, string> parse_key_value_string(const string &input, char delimiter);