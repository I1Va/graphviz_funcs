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
