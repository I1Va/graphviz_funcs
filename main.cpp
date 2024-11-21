#include <cassert>
#include <cstdlib>
#include <string.h>

#include "string_funcs.h"
#include "graphviz_funcs.h"
#include "general.h"

bool logs_ctor(log_t *log_obj, const char log_dir[], const char log_file[]) {
    *log_obj = {};
    memcpy(log_obj->log_file, log_file, MAX_LOG_FILE_PATH_SZ);
    memcpy(log_obj->log_dir, log_dir, MAX_LOG_DIR_SZ);

    create_logs_dir(log_obj->log_dir);
    FILE *log_file_ptr = fopen(log_file, "a");
    if (log_file_ptr == NULL) {
        debug("can't open '%s'", log_file);
        return false;
    }
    log_obj->log_file_ptr = log_file_ptr;

    fprintf(log_file_ptr, "<pre>\n");

    // GRAPH LOG
    log_obj->graph_log = DL_list_make_graphviz_dirs(log_obj->log_file);
    log_obj->graph_log.graph_num = get_dir_files_count(log_obj->graph_log.graphviz_codes_dir);

    snprintf(log_obj->graph_log.graphviz_code_file_name, MAX_LOG_FILE_PATH_SZ, "%s/%d.dot", log_obj->graph_log.graphviz_codes_dir, log_obj->graph_log.graph_num);
    snprintf(log_obj->graph_log.img_file_name, MAX_LOG_FILE_PATH_SZ, "%s/%d.png", log_obj->graph_log.img_dir, log_obj->graph_log.graph_num);
    snprintf(log_obj->graph_log.short_img_path, MAX_LOG_FILE_PATH_SZ, "%s/%d.png", LOG_IMG_DIR_NAME, log_obj->graph_log.graph_num);
    snprintf(log_obj->graph_log.short_img_path, MAX_LOG_FILE_PATH_SZ, "%s/%d.png", LOG_IMG_DIR_NAME, log_obj->graph_log.graph_num);

    FILE* graphviz_code_file = fopen(log_obj->graph_log.graphviz_code_file_name, "w");
    if (graphviz_code_file == NULL) {
        return false;
    }

    log_obj->graph_log.graphviz_code_file = graphviz_code_file;

    return true;
}

void fprintf_node(FILE *stream, DL_list_node_t *node) {
    fprintf(stream, "ptr: [%p], e%d, val{%d}\n", node, node->empty, node->value);
}

void DL_list_fprintf_border(FILE* stream, const char bord_char, const size_t bord_sz, bool new_line) {
    fprintf(stream, HTML_BLACK);
    for (size_t i = 0; i < bord_sz; i++) {
        fputc(bord_char, stream);
    }
    if (new_line) {
        fputc('\n', stream);
    }
}

void DL_list_fprintf_title(FILE *stream, const char tittle[], const char bord_char, const size_t bord_sz) {
    assert(tittle != NULL);
    size_t tittle_sz = strlen(tittle);
    if (bord_sz < tittle_sz) {
        return;
    }
    size_t len = bord_sz - tittle_sz;
    DL_list_fprintf_border(stream, bord_char, len / 2, false);
    fprintf_html_red(stream, "%s", tittle);
    DL_list_fprintf_border(stream, bord_char, (len + 1) / 2, true);
}

const char *DL_list_get_log_descr(enum DL_list_log_type_t log_type) {
    // #define DL_DESCR_(log_type) case log_type: return #log_type;

    switch (log_type) {
        case DL_LOG_ANALYS: return "DL_LOG_ANALYS";
        case DL_LOG_DEBUG: return "DL_LOG_DEBUG";
        case DL_LOG_ERROR: return "DL_LOG_ERROR";
        default: return "STRANGE LOG_TYPE";
    }
    // #undef DL_DESCR_
}

void DL_list_log_print_time(FILE *log_output_file_ptr) {
    time_t cur_time;
    time(&cur_time);
    struct tm *now = localtime(&cur_time);

    char date_str[date_nmemb] = {};
    char time_str[time_nmemb] = {};
    strftime(date_str, date_nmemb, "%m/%d/%Y", now);
    strftime(time_str, time_nmemb, "%T", now);

    double milliseconds = ((double) clock()) / CLOCKS_PER_SEC;
    fprintf(log_output_file_ptr, "Date: %s Time: %s Ms: {%f}\n", date_str, time_str, milliseconds);
}

void DL_list_print_log_func_info(FILE *log_output_file_ptr, const char file_name[], const char func_name[], const int line_idx) {
    fprintf(log_output_file_ptr, "file : {%s}; func: {%s}; line: {%d}\n", file_name, func_name, line_idx);
}

void DL_list_print_log_type(FILE *log_output_file_ptr, enum DL_list_log_type_t log_type) {
    fprintf(log_output_file_ptr, "log_type: {%s}\n", DL_list_get_log_descr(log_type));
}

void DL_list_log_var_print(FILE *log_output_file_ptr, enum DL_list_log_type_t log_type, const char file_name[], const char func_name[], const int line_idx, const char fmt[], ...) {
    DL_list_fprintf_title(log_output_file_ptr, "LOG_VAR", '-', BORDER_SZ);
    DL_list_print_log_type(log_output_file_ptr, log_type);
    DL_list_log_print_time(log_output_file_ptr);
    DL_list_print_log_func_info(log_output_file_ptr, file_name, func_name, line_idx);

    if (line_idx) {}

    va_list args;
    va_start(args, fmt);
    vfprintf(log_output_file_ptr, fmt, args);

    va_end(args);
    fprintf(log_output_file_ptr, "\n");

    DL_list_fprintf_border(log_output_file_ptr, '-', BORDER_SZ, true);
}

int get_dir_files_count(const char dir_path[]) {
    int file_count = 0;

    DIR *dirp;
    struct dirent *entry;

    dirp = opendir(dir_path);
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) {
            file_count++;
        }
    }
    closedir(dirp);
    return file_count;
}

graphviz_dir_t DL_list_make_graphviz_dirs(char log_file_path[]) {
    graphviz_dir_t logs_dir_obj = {};

    char *log_file_path_ptr = (char *) log_file_path;
    char *log_dir_ptr = strrchr(log_file_path_ptr, '/');
    memcpy(logs_dir_obj.log_dir, log_file_path, (size_t) (log_dir_ptr - log_file_path) * sizeof(char));


    snprintf(logs_dir_obj.img_dir, MAX_IMG_DIR_SZ, "%s/%s", logs_dir_obj.log_dir, LOG_IMG_DIR_NAME);
    snprintf(logs_dir_obj.graphviz_codes_dir, MAX_GRAPHVIZ_CODES_DIR_SZ, "%s/%s", logs_dir_obj.log_dir, LOG_GRAPHVIZ_CODE_DIR_NAME);

    char mkdir_img_command[MAX_SYSTEM_COMMAND_SIZE] = {};
    snprintf(mkdir_img_command, MAX_SYSTEM_COMMAND_SIZE, "mkdir -p %s", logs_dir_obj.img_dir);
    if (system(mkdir_img_command) != 0) {
        DEBUG_DL_LIST_ERROR(DL_ERR_SYSTEM, "execution: '%s' failed", mkdir_img_command);
    }

    char mkdir_graphviz_code_command[MAX_SYSTEM_COMMAND_SIZE] = {};
    snprintf(mkdir_graphviz_code_command, MAX_SYSTEM_COMMAND_SIZE, "mkdir -p %s", logs_dir_obj.graphviz_codes_dir);
    if (system(mkdir_graphviz_code_command) != 0) {
        DEBUG_DL_LIST_ERROR(DL_ERR_SYSTEM, "execution: '%s' failed", mkdir_graphviz_code_command);
    }

    return logs_dir_obj;
}

void graphviz_start_graph(FILE *graphviz_code_file) {
    fprintf(graphviz_code_file, "digraph G{\n");
    fprintf(graphviz_code_file, "    rankdir=LR;\n");
    // fprintf(graphviz_code_file, "    splines=true;\n");
    // fprintf(graphviz_code_file, "    overlap=true;\n");
}

void graphviz_end_graph(FILE *graphviz_code_file) {
    fprintf(graphviz_code_file, "}");
    fclose(graphviz_code_file);
}

void graphviz_make_node(FILE *graphviz_code_file, DL_list_node_t *node) {
    fprintf(graphviz_code_file, "    NODE%p[fillcolor=\"%s\",style=\"filled\",pin=true,shape=\"box\","
        "label=\"addr: %p\nval: %d\nprev: %p\nnext: %p\"];\n", node, EXIST_NODE_COLOR, node, node->value, node->prev, node->next);
}

void graphviz_fillcolor_node(FILE *graphviz_code_file, DL_list_node_t *node, const char color[]) {
    fprintf(graphviz_code_file, "    NODE%p[style=\"filled\",fillcolor=\"%s\"];\n", node, color);
}

void graphviz_make_tiny_node(FILE *graphviz_code_file, DL_list_node_t *node) {
    fprintf(graphviz_code_file, "    NODE%p[pin=true,shape=\"box\",label=\"val: %d\"];\n", node, node->value);
}

void graphviz_make_heavy_unvisible_edge(FILE *graphviz_code_file, DL_list_node_t *node1, DL_list_node_t *node2) {
    fprintf(graphviz_code_file, "    NODE%p -> NODE%p [weight=%d,color=\"white\"];\n", node1, node2, EDGE_MAX_WEIGHT);
}
void graphviz_make_edge(FILE *graphviz_code_file, DL_list_node_t *node1, DL_list_node_t *node2, const char color[] = "black", int penwidth=SIMP_EDGE_WIDTH) {
    fprintf(graphviz_code_file, "    NODE%p -> NODE%p [color=\"%s\",penwidth=%d];\n", node1, node2, color, penwidth);
}

void DL_list_log_html_insert_image(FILE *log_output_file_ptr, char short_img_path[], int width_percent) {
    // img_path = strrchr(img_path, '/');
    // img_path = strrchr(img_path, '/');
    fprintf(log_output_file_ptr, "<img src=\"%s\" width=\"%d%%\">\n", short_img_path, width_percent);
}

void graphviz_make_list_same_rank(FILE *graphviz_code_file, DL_list_t *list) {
    fprintf(graphviz_code_file, "    {rank = same; ");
    for (size_t i = 0; i < list->size; i++) {
        fprintf(graphviz_code_file, "NODE%p; ", &list->data[i]);
    }
    fprintf(graphviz_code_file, "};\n");
}

bool DL_list_generate_graph_dot(DL_list_t *list, log_t *log_obj) {
    assert(list != NULL);
    assert(log_obj != NULL);
    for (size_t i = 0; i < list->size; i++) {
        if (log_obj->short_log) {
            graphviz_make_tiny_node(log_obj->graph_log.graphviz_code_file, &list->data[i]);
        } else {
            graphviz_make_node(log_obj->graph_log.graphviz_code_file, &list->data[i]);
        }
        if (list->data[i].empty) {
            graphviz_fillcolor_node(log_obj->graph_log.graphviz_code_file, &list->data[i], EMPTY_NODE_COLOR);
        }
        if (i == 0) {
            graphviz_fillcolor_node(log_obj->graph_log.graphviz_code_file, &list->data[i], FICTIVE_NODE_COLOR);
        }
    }
    // graphviz_make_list_same_rank(graphviz_code_file, list);
    for (size_t i = 1; i < list->size; i++) {
        graphviz_make_heavy_unvisible_edge(log_obj->graph_log.graphviz_code_file, &list->data[i - 1], &list->data[i]);
    }

    for (size_t i = 0; i < list->size; i++) {
        if (list->data[i].empty && list->data[i].next != NULL) {
            printf("%lu\n", i);
            graphviz_make_edge(log_obj->graph_log.graphviz_code_file, &list->data[i], list->data[i].next, "green", 2);
            continue;
        }
        if (list->data[i].next != NULL) {
            graphviz_make_edge(log_obj->graph_log.graphviz_code_file, &list->data[i], list->data[i].next, "red", 2);
        }
        if (list->data[i].prev != NULL) {
            graphviz_make_edge(log_obj->graph_log.graphviz_code_file, list->data[i].prev, &list->data[i], "blue", 1);
        }
    }
    return true;
}

bool create_logs_dir(const char log_dir[]) {
    char mkdir_command[MAX_SYSTEM_COMMAND_SIZE] = {};
    snprintf(mkdir_command, MAX_SYSTEM_COMMAND_SIZE, "mkdir -p %s", log_dir);
    if (system(mkdir_command) != 0) {
        DEBUG_DL_LIST_ERROR(DL_ERR_SYSTEM, "execution: '%s' failed", mkdir_command);
        return false;
    }
    return true;
}

void DL_list_log_dump(DL_list_t *list, log_t *log_obj, const char file_name[], const char func_name[], const int line_idx) {
    if (list == NULL) {
        return;
    }

    if (log_obj->log_file_ptr == NULL) {
        return;
    }

    DL_list_fprintf_title(log_obj->log_file_ptr, "DL_LIST DUMP", '-', BORDER_SZ);
    DL_list_print_log_type(log_obj->log_file_ptr, DL_LOG_DEBUG);
    DL_list_log_print_time(log_obj->log_file_ptr);
    DL_list_print_log_func_info(log_obj->log_file_ptr, file_name, func_name, line_idx);

    fprintf_html_red(log_obj->log_file_ptr, "list [%p] at %s:%d\n", list, file_name, line_idx);
    fprintf_html_grn(log_obj->log_file_ptr, "size: [%lu]\n", list->size);
    fprintf_html_grn(log_obj->log_file_ptr, "free_addr: [%p]\n", list->free_node);

    char draw_graph_command[MAX_SYSTEM_COMMAND_SIZE] = {};
    snprintf(draw_graph_command, MAX_SYSTEM_COMMAND_SIZE, "dot %s -Tpng -o %s",
        log_obj->graph_log.graphviz_code_file_name, log_obj->graph_log.img_file_name);
    if (system(draw_graph_command) != 0) {
        DEBUG_DL_LIST_ERROR(DL_ERR_SYSTEM, "execution: '%s' failed", draw_graph_command);
        return;
    }


    DL_list_log_html_insert_image(log_obj->log_file_ptr, log_obj->graph_log.short_img_path, LOG_WIDTH_VAL);
    // for (int i = 0; i < list.size; i++) {
    //     fprintf(list.log_output_file_ptr, list.data)
    // }

    DL_list_fprintf_border(log_obj->log_file_ptr, '-', BORDER_SZ, true);
}

void dot_code_render(dot_code_t *dot_code) {

}

int main() {
    dot_code_t dot_code = {};
    dot_code_t_ctor(&dot_code);

    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bara");
    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bara");
    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bara");
    dot_new_node(&dot_code, DEFAULT_NODE_PARS, "bere");

    dot_new_edge(&dot_code, 0, 1, DEFAULT_EDGE_PARS, "boom");


}