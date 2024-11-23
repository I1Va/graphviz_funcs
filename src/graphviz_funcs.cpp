#include <stdlib.h>
#include <assert.h>

#include "general.h"
#include "graphviz_funcs.h"


bool dot_code_t_ctor(dot_code_t *dot_code, dot_code_pars_t pars) {
    assert(dot_code != NULL);

    (*dot_code).pars = pars;

    (*dot_code).node_list_sz = 0;

    (*dot_code).edge_list_sz = 0;

    return true;
}

void dot_code_t_dtor(dot_code_t *dot_code) {
    if (dot_code == NULL) {
        return;
    }

}

void dot_write_node(FILE *dot_code_file, dot_node_t *node) {
    assert(node != NULL);
    assert(dot_code_file != NULL);

    fprintf(dot_code_file, "    NODE%p[", node);
    if (node->pars.shape) {
        fprintf(dot_code_file, "shape=\"%s\"", node->pars.shape);
    }
    if (node->pars.color) {
        fprintf(dot_code_file, ",color=\"%s\"", node->pars.color);
    }
    if (node->pars.fillcolor) {
        fprintf(dot_code_file, ",fillcolor=\"%s\"", node->pars.fillcolor);
    }
    if (node->pars.shape) {
        fprintf(dot_code_file, ",shape=\"%s\"", node->pars.shape);
    }
    if (node->label) {
        fprintf(dot_code_file, ",label=\"%s\"", node->label);
    }
    fprintf(dot_code_file, "]\n");
}

size_t dot_new_node(dot_code_t *dot_code, dot_node_pars_t pars, char *label) {
    if (dot_code->node_list_sz >= dot_code->node_list_cap) {
        debug("node_list overflow");
        return 0;
    }
    dot_code->node_list[dot_code->node_list_sz++] = {pars, label};
    return dot_code->node_list_sz - 1;
}

bool dot_new_edge(dot_code_t *dot_code, size_t node1_idx, size_t node2_idx, dot_edge_pars_t pars, char *label) {
    if (dot_code->edge_list_sz >= dot_code->edge_list_cap) {
        debug("edge_list overflow");
        return false;
    }
    dot_code->edge_list[dot_code->edge_list_sz++] =
        {
            &dot_code->node_list[node1_idx],&dot_code->node_list[node2_idx],  pars, label
        };

    return true;
}
