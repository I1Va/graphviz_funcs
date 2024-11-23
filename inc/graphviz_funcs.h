#ifndef GRAPHVIZ_FUNCS_H
#define GRAPHVIZ_FUNCS_H

#include <string.h>


struct dot_code_pars_t {
    char *rankdir;
};

struct dot_node_pars_t {
    char *shape;
    char *color;
    char *fillcolor;
    char *style;
};


struct dot_node_t {
    dot_node_pars_t pars;

    char *label;
};

struct dot_edge_pars_t {
    char *color;
    size_t penwidth;
};

struct dot_edge_t {
    void *start;
    void *end;

    dot_edge_pars_t pars;

    char *label;
};

const size_t DOT_CODE_LIST_MAX_SZ = 128;
const size_t STR_STORAGE_CHUNK_SIZE = 128;

const dot_node_pars_t DEFAULT_NODE_PARS = {"tab", "red", "black", "filled"};
const dot_edge_pars_t DEFAULT_EDGE_PARS = {"cyan", 2};
const dot_code_pars_t LIST_DOT_CODE_PARS = {"LR"};

struct dot_code_t {
    dot_code_pars_t pars;

    dot_node_t node_list[DOT_CODE_LIST_MAX_SZ];
    size_t node_list_cap = DOT_CODE_LIST_MAX_SZ;
    size_t node_list_sz;

    dot_edge_t edge_list[DOT_CODE_LIST_MAX_SZ];
    size_t edge_list_cap = DOT_CODE_LIST_MAX_SZ;
    size_t edge_list_sz;
};

bool dot_code_t_ctor(dot_code_t *dot_code, dot_code_pars_t pars);
void dot_code_t_dtor(dot_code_t *dot_code);
size_t dot_new_node(dot_code_t *dot_code, dot_node_pars_t pars, char *label);
bool dot_new_edge(dot_code_t *dot_code, size_t node1_idx, size_t node2_idx, dot_edge_pars_t pars, char *label);


#endif // GRAPHVIZ_FUNCS_H