#ifndef DOT_LOG_H
#define DOT_LOG_H

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#include "general.h"
#include "graphviz_funcs.h"


size_t get_max_str_len(size_t n, ...);
void fprintf_str_block(FILE *stream, const size_t indent_sz, const size_t block_sz, const char *string);
void fprintf_indent_str(FILE *stream, const size_t indent_sz, const char fmt[], ...);
void dot_code_t_dump(FILE *log_file, dot_code_t *dot_code);

#endif // DOT_LOG_H
