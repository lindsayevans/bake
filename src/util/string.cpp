#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

void replace_all(string &str, const string &from, const string &to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

map<string, string> parse_key_value_string(const string &input, char delimiter)
{
    map<string, string> result;
    stringstream ss(input);
    string token;

    while (getline(ss, token, delimiter))
    {
        size_t pos = token.find('=');
        if (pos != string::npos)
        {
            string key = token.substr(0, pos);
            string value = token.substr(pos + 1);
            result[key] = value;
        }
    }

    return result;
}