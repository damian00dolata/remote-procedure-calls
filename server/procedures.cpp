#include "procedures.hpp"

std::string exec(const char* cmd) {
    char buf[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buf, sizeof buf, pipe) != NULL) {
            result += buf;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_dollar = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    // Count elements
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_dollar = tmp;
        }
        tmp++;
    }

    // Adding space
    count += last_dollar < (a_str + strlen(a_str) - 1);

    count++;

    result = (char**)malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            //assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        //assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}