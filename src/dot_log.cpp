#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#include "general.h"
#include "graphviz_funcs.h"

size_t get_max_str_len(size_t n, ...) {
    size_t max_str_len = 0;
    va_list factor;
    va_start(factor, n);

    for(size_t i = 0; i < n; i++) {
        char *str_ptr = va_arg(factor, char *); // FIXME: почему, при strlen(va_arg(factor, char *)) возникает SEGV?
        max_str_len = MAX(max_str_len, strlen(str_ptr));
    }

    va_end(factor);
    return max_str_len;
}

void fprintf_str_block(FILE *stream, const size_t indent_sz, const size_t block_sz, char *string) {
    assert(string != NULL);

    int str_len = (int) strlen(string);

    if ((int) block_sz < str_len) {
        debug("string crawls out of the block : string[%d] : '%s', block_sz : [%d]", str_len, string, block_sz);
        str_len = (int) block_sz;
    }
    fprintf(stream, "%*.s", (int) indent_sz, "");
    fprintf(stream, "%*s%*.s", str_len, string, (int) block_sz - str_len, "");
}

void fprintf_indent_str(FILE *stream, const size_t indent_sz, const char fmt[], ...) {
    va_list args;
    fprintf(stream, "%*.s", (int) indent_sz, "");
    va_start(args, fmt);
    vfprintf(stream, fmt, args);

    va_end(args);
}

void dot_code_t_dump(FILE *log_file, dot_code_t *dot_code) {
    assert(log_file != NULL);
    assert(dot_code != NULL);

    char bufer[BUFSIZ] = {};
    size_t indent_sz = 4;
    size_t indent_step = 4;

    fprintf(log_file, "dot_code_t\n{\n");

    size_t dot_code_pars_block_sz = get_max_str_len(5,
        "rankdir_",
        "node_list_cap_", "node_list_sz_",
        "edge_list_cap_", "edge_list_sz_"
    );

    fprintf_str_block(log_file, indent_sz, dot_code_pars_block_sz, "rankdir_");
    fprintf(log_file, " = '%s'\n\n", dot_code->pars.rankdir);

    fprintf_str_block(log_file, indent_sz, dot_code_pars_block_sz, "node_list_cap_");
    fprintf(log_file, " = %lu\n", dot_code->node_list_cap);

    fprintf_str_block(log_file, indent_sz, dot_code_pars_block_sz, "node_list_sz_");
    fprintf(log_file, " = %lu\n", dot_code->node_list_sz);

    fprintf_indent_str(log_file, indent_sz, "node_list_ = \n");
    fprintf_indent_str(log_file, indent_sz, "{\n");

    for (size_t i = 0; i < dot_code->node_list_sz; i++) {
        fprintf_indent_str(log_file, indent_sz + indent_step, "");
        dot_write_node(log_file, &dot_code->node_list[i]);
    }
    fprintf_indent_str(log_file, indent_sz, "};\n");

    fprintf(log_file, "}\n");
}
