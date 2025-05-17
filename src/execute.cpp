#include <cstdlib>
#include <iostream>
#include <queue>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>

#include "arguments.hpp"
#include "config.hpp"
#include "execute.hpp"
#include "util/string.hpp"

using namespace std;

FILE *create_temp_file(char *name)
{
    int fd = mkstemp(name);
    FILE *fp = 0;
    if (fd >= 0)
    {
        fp = fdopen(fd, "w+");
        if (fp == 0)
        {
            close(fd);
            unlink(name);
        }
    }
    return (fp);
}

struct execution_result execute_command(string command)
{

    char out_file_name[] = "bake_cmd_out.XXXXXX";
    char err_file_name[] = "bake_cmd_err.XXXXXX";
    create_temp_file(out_file_name);
    create_temp_file(err_file_name);

    ostringstream ss;
    ss << command.c_str() << " >" << out_file_name << " 2>" << err_file_name;
    const auto cmd = ss.str();

    const auto status = system(cmd.c_str());

    ostringstream out_stream;
    ifstream out_file_stream;
    out_file_stream.open(out_file_name);
    out_stream << out_file_stream.rdbuf();
    out_file_stream.close();
    remove(out_file_name);

    ostringstream err_stream;
    ifstream err_file_stream;
    err_file_stream.open(err_file_name);
    err_stream << err_file_stream.rdbuf();
    err_file_stream.close();
    remove(err_file_name);

    execution_result result;
    result.exit_status = WEXITSTATUS(status);
    result.out = out_stream.str();
    result.err = err_stream.str();

    return result;
}

bool execute_command_queue(queue<string> command_queue, struct config config, struct program_arguments args)
{
    bool success = true;
    while (!command_queue.empty())
    {
        string command = command_queue.front();
        if (!args.quiet)
        {
            cout << "Executing command: " << command << endl;
        }
        struct execution_result result = execute_command(command.c_str());
        if (result.exit_status != 0)
        {
            cout << endl
                 << result.err;
            cout
                << RED << "Command exited with status " << static_cast<int16_t>(result.exit_status) << RESET << endl;
            cout << RED << "See above for error output" RESET << endl;
            success = false;
            break;
            ;
        }
        else
        {
            if (!args.quiet)
            {
                cout << endl
                     << result.out << endl;
                cout << "Command executed sucessfully" << endl;
            }
        }

        command_queue.pop();
    }

    return success;
}
