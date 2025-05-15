#pragma once

#include <vector>

#include "arguments.hpp"

#ifndef DEBUG
#define DEBUG false
#endif

#define PROGRAM_NAME "bake"
#define PROGRAM_VERSION_STRING "0.0.1-alpha"

using namespace std;

vector<string> merge_targets(vector<string> default_targets, vector<string> provided_targets);
