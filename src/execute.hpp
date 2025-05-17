#pragma once

#include <queue>
#include <string>

#include "arguments.hpp"
#include "config.hpp"

using namespace std;

struct execution_result
{
    int8_t exit_status;
    string out;
    string err;
};

bool execute_command_queue(queue<string> command_queue, struct config config, struct program_arguments args);
