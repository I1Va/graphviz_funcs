#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string.h>

#include "string_funcs.h"
#include "graphviz_funcs.h"
#include "general.h"

const size_t MAX_DOT_DIR_SZ = 128;
const size_t MAX_DOT_FILE_NAME_SZ = 128;
const size_t MAX_DOT_IMG_NAME_SZ = 128;

struct dot_dir_t {
    char dot_dir[MAX_DOT_DIR_SZ];
    char dot_code_file_path[MAX_DOT_FILE_NAME_SZ];
    char dot_img_path[MAX_DOT_IMG_NAME_SZ];

    FILE* dot_code_file;
};

bool dot_dir_ctor(dot_dir_t *log_obj, const char dot_dir_path[], const char dot_file_name[], const char dot_img_name[]) {
    memcpy(log_obj->dot_dir, dot_dir_path, MAX_DOT_DIR_SZ);
    snprintf(log_obj->dot_code_file_path, MAX_DOT_FILE_NAME_SZ, "%s/%s", dot_dir_path, dot_file_name);
    snprintf(log_obj->dot_img_path, MAX_DOT_IMG_NAME_SZ, "%s/%s", dot_dir_path, dot_img_name);

    log_obj->dot_code_file = fopen(log_obj->dot_code_file_path, "w");
    if (log_obj->dot_code_file == NULL) {
        debug("can't open '%s'", log_obj->dot_code_file_path);
        return false;
    }

    return true;
}

void dot_dir_dtor(dot_dir_t *log_obj) {
    if (!log_obj) {
        return;
    }
    if (log_obj->dot_code_file) {
        fclose(log_obj->dot_code_file);
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

void dot_code_render(dot_dir_t *dot_dir, dot_code_t *dot_code) {
    dot_start_graph(dot_dir->dot_code_file, &dot_code->pars);
    dot_end_graph(dot_dir->dot_code_file);
}

// Модуль не несет ответственности за актуальность строковых указателей

const char DOT_DIR_PATH[] = "./logs/dot";
const char DOT_FILE_NAME[] = "graph.dot";
const char DOT_IMG_NAME[] = "gr_img.png";

int main() {
    dot_code_t dot_code = {};
    dot_code_t_ctor(&dot_code, LIST_DOT_CODE_PARS);

    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bara");
    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bara");
    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bara");
    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bere");

    dot_new_edge(&dot_code, 0, 1, DEFAULT_EDGE_PARS, "boom");

    dot_dir_t dot_dir = {};
    dot_dir_ctor(&dot_dir, DOT_DIR_PATH, DOT_FILE_NAME, DOT_IMG_NAME);


}