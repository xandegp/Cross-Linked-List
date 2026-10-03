/* ============================================================================
    Crossed Linked-List
 ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------
  Data Structures
 ---------------------------------------------------------------------- */

typedef struct cell {
    int line;
    int col;
    int value;
    struct cell *next_line;
    struct cell *next_col;
} cell_t;

typedef struct row {
    int index;
    cell_t* first;
    struct row *next;
} row_t;

typedef struct {
    bool is_transpose;
    int line;
    int col;
    union {
        int previous_value;
        int size;
    };
} operation_t;

typedef struct stack_node {
    operation_t op;
    struct stack_node *next;
} stack_node_t;

typedef struct {
    stack_node_t* top;
} stack_t;

typedef struct {
    row_t* first_line;
    row_t* first_col;
    int total_cells;
    stack_t history;
} spreadsheet_t;

/* ---------------------------------------------------------------------- *
    Helper function for command string comparison
 ---------------------------------------------------------------------- */

int equals(char* a, char* b) {
    int i = 0;
    while (a[i] == b[i] && a[i] != '\0') i++;

    return (a[i] == b[i]);
}

/* ---------------------------------------------------------------------- *
    Spreadsheet Operations
 ---------------------------------------------------------------------- */

void start_spreadsheet(spreadsheet_t *p) {
    p->first_line = NULL;
    p->first_col = NULL;
    p->total_cells = 0;
    p->history.top = NULL;
}

cell_t* search_cell(spreadsheet_t *p, int line, int col,
                    cell_t** prev_cell_line, cell_t** prev_cell_col,
                    row_t** prev_row_line, row_t** prev_row_col) {
    cell_t* current = NULL;
    cell_t* temp_line = NULL;
    cell_t* temp_col = NULL;

    if (prev_cell_line != NULL) *prev_cell_line = NULL;
    if (prev_cell_col != NULL) *prev_cell_col = NULL;
    if (prev_row_line != NULL) *prev_row_line = NULL;
    if (prev_row_col != NULL) *prev_row_col = NULL;

    if (line < 0 || col < 0 || p == NULL) return NULL;

    row_t *curr_row_line = p->first_line;
    row_t *curr_row_col = p->first_col;

    while (curr_row_line != NULL && curr_row_line->index < line) {
        if (prev_row_line) *prev_row_line = curr_row_line;
        curr_row_line = curr_row_line->next;
    }
    while (curr_row_col != NULL && curr_row_col->index < col) {
        if (prev_row_col) *prev_row_col = curr_row_col;
        curr_row_col = curr_row_col->next;
    }

    if (curr_row_line != NULL && curr_row_line->index == line) {
        temp_line = curr_row_line->first;
        while (temp_line != NULL && temp_line->col < col) {
            if (prev_cell_line) *prev_cell_line = temp_line;
            temp_line = temp_line->next_col;
        }
    }

    if (temp_line != NULL && temp_line->col == col) current = temp_line;

    if (curr_row_col != NULL && curr_row_col->index == col) {
        temp_col = curr_row_col->first;
        while (temp_col != NULL && temp_col->line < line) {
            if (prev_cell_col) *prev_cell_col = temp_col;
            temp_col = temp_col->next_line;
        }
    }
    
    return current;
}

int get_value(spreadsheet_t *p, int line, int col) {
    if (p == NULL) return 0;

    cell_t* current = search_cell(p, line, col, NULL, NULL, NULL, NULL);

    if (current != NULL) return current->value;

    return 0;
}

int sum_range(spreadsheet_t* p, int start_line, int end_line, int start_col, int end_col) {
    if (p == NULL || start_line < 0 || end_line < start_line || start_col < 0 || end_col < start_col) return 0;
    
    int sum = 0;
    row_t* curr_row_line = p->first_line;
    
    while (curr_row_line != NULL && curr_row_line->index < start_line) {
        curr_row_line = curr_row_line->next;
    }

    while (curr_row_line != NULL && curr_row_line->index <= end_line) {
        cell_t* current = curr_row_line->first;

        while (current != NULL && current->col < start_col) {
            current = current->next_col;
        }

        while (current != NULL && current->col <= end_col) {
            sum += current->value;
            current = current->next_col;
        }
        curr_row_line = curr_row_line->next;
    }
    return sum;
}

int count_non_null(spreadsheet_t* p) {
    if (p == NULL) return 0;

    int count = 0;
    row_t* curr_row_line = p->first_line;

    while (curr_row_line != NULL) {
        cell_t* current = curr_row_line->first;

        while (current != NULL) {
            count++;
            current = current->next_col;
        }

        curr_row_line = curr_row_line->next;
    } 
    return count;
}

bool define_cell(spreadsheet_t* p, int line, int col, int value) {
    if (p == NULL || line < 0 || col < 0) return false;

    cell_t *prev_cell_line, *prev_cell_col;
    row_t *prev_row_line, *prev_row_col;

    cell_t* current = search_cell(p, line, col, &prev_cell_line, &prev_cell_col,
                                  &prev_row_line, &prev_row_col);
    int prev_val;
    
    if (current) prev_val = current->value;
    else prev_val = 0;

    if (prev_val == value) return false;

    stack_node_t* new_node = (stack_node_t*) malloc(sizeof(stack_node_t));
    if (!new_node) return false;
    new_node->op.line = line;
    new_node->op.col = col;
    new_node->op.is_transpose = false;
    new_node->op.previous_value = prev_val;
    new_node->next = p->history.top;
    p->history.top = new_node;

    if (current != NULL && value != 0) {
        current->value = value;
    }
    else if (current == NULL && value != 0) {
        cell_t* new_cell = (cell_t*) malloc(sizeof(cell_t));
        if (!new_cell) return false;
        new_cell->col = col;
        new_cell->line = line;
        new_cell->value = value;
        
        row_t* new_row_line;
        row_t* new_row_col;

        if (prev_row_line) new_row_line = prev_row_line->next;
        else new_row_line = p->first_line;

        if (new_row_line == NULL || new_row_line->index != line) {
            new_row_line = (row_t*) malloc(sizeof(row_t));
            if (!new_row_line) { free(new_cell); return false; }
            new_row_line->index = line;
            new_row_line->first = NULL;
            
            if (prev_row_line == NULL) {
                new_row_line->next = p->first_line;
                p->first_line = new_row_line;                
            }
            else {
                new_row_line->next = prev_row_line->next;
                prev_row_line->next = new_row_line;
            }
        }

        if (prev_row_col) new_row_col = prev_row_col->next;
        else new_row_col = p->first_col;

        if (new_row_col == NULL || new_row_col->index != col) {
            new_row_col = (row_t*) malloc(sizeof(row_t));
            if (!new_row_col) { free(new_cell); return false; }
            new_row_col->index = col;
            new_row_col->first = NULL;
            
            if (prev_row_col == NULL) {
                new_row_col->next = p->first_col;
                p->first_col = new_row_col;                
            }
            else {
                new_row_col->next = prev_row_col->next;
                prev_row_col->next = new_row_col;
            }
        }

        if (prev_cell_line) {
            new_cell->next_col = prev_cell_line->next_col;
            prev_cell_line->next_col = new_cell;
        }
        else {
            new_cell->next_col = new_row_line->first;
            new_row_line->first = new_cell;
        }

        if (prev_cell_col) {
            new_cell->next_line = prev_cell_col->next_line;
            prev_cell_col->next_line = new_cell;
        }
        else {
            new_cell->next_line = new_row_col->first;
            new_row_col->first = new_cell;
        }

        p->total_cells++;
    }
    else {
        row_t* curr_row_line;
        row_t* curr_row_col;
        
        if (prev_row_line) curr_row_line = prev_row_line->next;
        else curr_row_line = p->first_line;

        if (prev_row_col) curr_row_col = prev_row_col->next;
        else curr_row_col = p->first_col;

        if (prev_cell_line) prev_cell_line->next_col = current->next_col;
        else curr_row_line->first = current->next_col;

        if (prev_cell_col) prev_cell_col->next_line = current->next_line;
        else curr_row_col->first = current->next_line;

        if (curr_row_line->first == NULL) {
            if (prev_row_line) prev_row_line->next = curr_row_line->next;
            else p->first_line = curr_row_line->next;
            free(curr_row_line);
        }

        if (curr_row_col->first == NULL) {
            if (prev_row_col) prev_row_col->next = curr_row_col->next;
            else p->first_col = curr_row_col->next;
            free(curr_row_col);
        }

        free(current);
        p->total_cells--;
    }
    return true;
}

bool remove_cell(spreadsheet_t* p, int line, int col) {
    if (p == NULL || line < 0 || col < 0) return false;
    
    cell_t *prev_cell_line, *prev_cell_col;
    row_t *prev_row_line, *prev_row_col, *curr_row_line, *curr_row_col;

    cell_t* current = search_cell(p, line, col, &prev_cell_line, &prev_cell_col,
                                  &prev_row_line, &prev_row_col);

    if (current == NULL) return false;

    stack_node_t* new_node = (stack_node_t*) malloc(sizeof(stack_node_t));
    if (!new_node) return false;
    new_node->op.col = col;
    new_node->op.line = line;
    new_node->op.is_transpose = false;
    new_node->op.previous_value = current->value;
    new_node->next = p->history.top;
    p->history.top = new_node;
    
    if (prev_row_line) curr_row_line = prev_row_line->next;
    else curr_row_line = p->first_line;
    
    if (prev_row_col) curr_row_col = prev_row_col->next;
    else curr_row_col = p->first_col;

    if (prev_cell_line) prev_cell_line->next_col = current->next_col;
    else curr_row_line->first = current->next_col;

    if (prev_cell_col) prev_cell_col->next_line = current->next_line;
    else curr_row_col->first = current->next_line;

    if (curr_row_line->first == NULL) {
        if (prev_row_line) prev_row_line->next = curr_row_line->next;
        else p->first_line = curr_row_line->next;
        free(curr_row_line);
    }

    if (curr_row_col->first == NULL) {
        if (prev_row_col) prev_row_col->next = curr_row_col->next;
        else p->first_col = curr_row_col->next;
        free(curr_row_col);
    }
    free(current);
    p->total_cells--;

    return true;
}

bool transpose(spreadsheet_t* p, int line, int col, int size) {
    if (p == NULL || line < 0 || col < 0 || size <= 0) return false;

    int swaps = 0;

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int l1 = line + i, c1 = col + j;
            int l2 = line + j, c2 = col + i;

            int val1 = get_value(p, l1, c1);
            int val2 = get_value(p, l2, c2);

            if (val1 != val2) {
                define_cell(p, l1, c1, val2);
                define_cell(p, l2, c2, val1);
                swaps++;
            }
        }
    }

    if (swaps == 0) return false;

    int to_remove = 2 * swaps;

    for (int k = 0; k < to_remove; k++) {
        if (p->history.top != NULL) {
            stack_node_t* temp = p->history.top;
            p->history.top = temp->next;
            free(temp);
        }
    }

    stack_node_t* new_node = (stack_node_t*) malloc(sizeof(stack_node_t));
    if (!new_node) return false;

    new_node->op.line = line;
    new_node->op.col = col;
    new_node->op.is_transpose = true;
    new_node->op.size = size;
    new_node->next = p->history.top;
    p->history.top = new_node;

    return true;
}

bool undo(spreadsheet_t* p) {
    if (p == NULL || !p->history.top) return false;     

    stack_node_t* remove_top = p->history.top;
    operation_t op = remove_top->op;

    p->history.top = remove_top->next;
    free(remove_top);

    int line = op.line;
    int col = op.col;
    int val = op.previous_value;

    if (op.is_transpose) {
        int size = op.size;
        transpose(p, line, col, size);

        stack_node_t* temp = p->history.top;
        if (temp) {
            p->history.top = temp->next;
            free(temp);
        }
    }
    else {
        define_cell(p, line, col, val);
        stack_node_t* temp = p->history.top;
        if (temp) {
            p->history.top = temp->next;
            free(temp);
        }
    }
    return true;
}

void show_spreadsheet(spreadsheet_t *p) {
    if (p == NULL) {
        printf("PLANILHA VAZIA\n");
        return; 
    }

    row_t* curr_row_line = p->first_line;

    while (curr_row_line != NULL) {
        cell_t* current = curr_row_line->first;
        while (current != NULL) {
            printf("%d %d %d\n", current->line, current->col, current->value);
            current = current->next_col;
        }
        curr_row_line = curr_row_line->next;
    }
}

void show_history(spreadsheet_t* p) {
    if (p == NULL || p->history.top == NULL) {
        printf("HISTORICO VAZIO\n");
        return;
    }

    stack_node_t* current = p->history.top;
    while (current) {
        if (!current->op.is_transpose) {
            printf("%d %d %d\n", current->op.line, current->op.col, current->op.previous_value);
        } else {
            printf("T %d %d %d\n", current->op.line, current->op.col, current->op.size);
        }
        current = current->next;
    }
}

void free_all(spreadsheet_t* p) {
    if (p == NULL) return;

    row_t* row_line = p->first_line;

    while (row_line != NULL) {
        cell_t* to_delete;
        cell_t* current = row_line->first;

        while (current != NULL) {
            to_delete = current;
            current = current->next_col;
            free(to_delete);
        }

        row_line = row_line->next;
    }

    row_t* row_line_delete;
    row_t* curr_row_line = p->first_line;
    while (curr_row_line != NULL) {
        row_line_delete = curr_row_line;
        curr_row_line = curr_row_line->next;
        free(row_line_delete);
    }
    
    row_t* row_col_delete;
    row_t* curr_row_col = p->first_col;
    while (curr_row_col != NULL) {
        row_col_delete = curr_row_col;
        curr_row_col = curr_row_col->next;
        free(row_col_delete);
    }

    stack_node_t* to_delete_node;
    stack_node_t* curr_node = p->history.top;
    while (curr_node != NULL) {
        to_delete_node = curr_node;
        curr_node = curr_node->next;
        free(to_delete_node);
    }
    p->history.top = NULL;
    p->first_col = NULL;
    p->first_line = NULL;
    p->total_cells = 0;
}

/* ---------------------------------------------------------------------- *
    Main Execution Block
 ---------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Use command: %s input_file.txt output_file.txt\n", argv[0]);
        return 1;
    }
    
    FILE* input = fopen(argv[1], "r");
    FILE* output = freopen(argv[2], "w", stdout);

    if (!input || !output) {
        fprintf(stderr, "Error opening files.\n");
        return 1;
    }

    spreadsheet_t p;
    start_spreadsheet(&p);

    int n;
    fscanf(input, "%d", &n);

    char cmd[20];

    while (fscanf(input, "%s", cmd) != EOF) {
 
        if (equals(cmd, "DEF")) {
            int line, col, value;
            fscanf(input, "%d %d %d", &line, &col, &value);
            define_cell(&p, line, col, value);
        } else if (equals(cmd, "REM")) {
            int line, col;
            fscanf(input, "%d %d", &line, &col);
            remove_cell(&p, line, col);
        } else if (equals(cmd, "GET")) {
            int line, col;
            fscanf(input, "%d %d", &line, &col);
            fprintf(output, "GET %d %d %d\n", line, col, get_value(&p, line, col));
        } else if (equals(cmd, "SUM")) {
            int li, lf, ci, cf;
            fscanf(input, "%d %d %d %d", &li, &lf, &ci, &cf);
            fprintf(output, "SUM %d %d %d %d %d\n", li, lf, ci, cf, sum_range(&p, li, lf, ci, cf));
        } else if (equals(cmd, "COUNT")) {
            fprintf(output, "COUNT %d\n", count_non_null(&p));
        } else if (equals(cmd, "UNDO")) {
            if (!undo(&p)) {
                fprintf(output, "EMPTY HISTORY\n");
            }
        } else if (equals(cmd, "SHOW")) {
            if (count_non_null(&p)) {
                printf("SPREADSHEET\n");
                show_spreadsheet(&p);
            } else {
                printf("EMPTY SPREADSHEET\n");
            }
        } else if (equals(cmd, "HISTORY")) {
            if (p.history.top) {
                printf("HISTORY\n");
                show_history(&p);
            }
            else {
                printf("EMPTY HISTORY\n");
            }
        } else if (equals(cmd, "TRANSPOSE")) {
            int line, col, size;
            fscanf(input, "%d %d %d", &line, &col, &size);
            transpose(&p, line, col, size);
        }
    }

    fclose(input);
    fclose(output);

    free_all(&p);
    return 0;
}