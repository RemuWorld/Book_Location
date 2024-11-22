#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

char* read_file(const char* filename);
void locate_by_code(const char* base_path, const char* code);
void search_by_name(const char* filename, const char* base_path);