#pragma once
#include <string>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

std::string exec(const char* cmd);
char** str_split(char* a_str, const char a_delim);