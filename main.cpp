#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "string_funcs.h"
#include "graphviz_funcs.h"
#include "general.h"

const size_t MAX_DOT_DIR_SZ = 128;
const size_t MAX_DOT_FILE_NAME_SZ = 128;
const size_t MAX_DOT_IMG_NAME_SZ = 128;
const size_t MAX_SYSTEM_COMMAND_SIZE = 128;

const char DOT_DIR_PATH[] = "./logs/dot";
const char DOT_FILE_NAME[] = "graph.dot";
const char DOT_IMG_NAME[] = "gr_img.png";

struct dot_dir_t {
    char dot_dir[MAX_DOT_DIR_SZ];
    char dot_code_file_path[MAX_DOT_FILE_NAME_SZ];
    char dot_img_path[MAX_DOT_IMG_NAME_SZ];

    FILE* dot_code_file;
};

bool dot_dir_ctor(dot_dir_t *dot_dir, const char dot_dir_path[], const char dot_file_name[], const char dot_img_name[]) {
    memcpy(dot_dir->dot_dir, dot_dir_path, MAX_DOT_DIR_SZ);
    snprintf(dot_dir->dot_code_file_path, MAX_DOT_FILE_NAME_SZ, "%s/%s", dot_dir_path, dot_file_name);
    snprintf(dot_dir->dot_img_path, MAX_DOT_IMG_NAME_SZ, "%s/%s", dot_dir_path, dot_img_name);

    dot_dir->dot_code_file = fopen(dot_dir->dot_code_file_path, "w");
    if (dot_dir->dot_code_file == NULL) {
        debug("can't open '%s'", dot_dir->dot_code_file_path);
        return false;
    }

    return true;
}

void dot_dir_dtor(dot_dir_t *dot_dir) {
    if (!dot_dir) {
        return;
    }
    if (dot_dir->dot_code_file) {
        fclose(dot_dir->dot_code_file);
    }
}

void dot_start_graph(FILE *dot_code_file, dot_code_pars_t *pars) {
    assert(pars != NULL);

    fprintf(dot_code_file, "digraph G{\n");
    if (pars->rankdir) {
        fprintf(dot_code_file, "    rankdir=%s;\n", pars->rankdir);
    }
}

void dot_end_graph(FILE *dot_code_file) {
    fprintf(dot_code_file, "}");
    fclose(dot_code_file);
}

void dot_draw_image(dot_dir_t *dot_dir) {
    char draw_graph_command[MAX_SYSTEM_COMMAND_SIZE] = {};
    snprintf(draw_graph_command, MAX_SYSTEM_COMMAND_SIZE, "dot %s -Tpng -o %s",
        dot_dir->dot_code_file_path, dot_dir->dot_img_path);
    if (system(draw_graph_command) != 0) {
        debug("system exectuion : '%s' failed", draw_graph_command);
        return;
    }
}

void dot_code_render(dot_dir_t *dot_dir, dot_code_t *dot_code) {
    dot_start_graph(dot_dir->dot_code_file, &dot_code->pars);
    for (size_t i = 0; i < dot_code->node_list_sz; i++) {
        dot_write_node(dot_dir->dot_code_file, &dot_code->node_list[i]);
    }
    dot_end_graph(dot_dir->dot_code_file);
    dot_draw_image(dot_dir);
}





// DOT_LOG_CPP











// Модуль не несет ответственности за актуальность строковых указателей

void test_mode_launch() {

}

int main() {
    // test_mode_launch();

    dot_code_t dot_code = {};
    dot_code_t_ctor(&dot_code, LIST_DOT_CODE_PARS);

    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bara");
    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bara");
    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bara");
    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bere");
    printf("max: %lu\n", get_max_str_len(3, dot_code.node_list[0].pars.color, dot_code.node_list[0].pars.style, dot_code.node_list[0].pars.shape));
    dot_new_edge(&dot_code, 0, 1, DEFAULT_EDGE_PARS, "boom");

    dot_dir_t dot_dir = {};
    dot_dir_ctor(&dot_dir, DOT_DIR_PATH, DOT_FILE_NAME, DOT_IMG_NAME);


    dot_code_t_dump(stdout, &dot_code);

    dot_code_render(&dot_dir, &dot_code);
}