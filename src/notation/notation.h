#ifndef __NOTATION__
#define __NOTATION__

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RES_OK 0
#define RES_ERR 1

#define OP_NONE '\0'
#define NUM_ZERO 0.0

#define OP_RANK_NONE 0

#define BKT_OPEN '('
#define BKT_CLOSE ')'

#define FUNC_COS 'c'
#define FUNC_SIN 's'
#define FUNC_TAN 't'
#define FUNC_ACOS '1'
#define FUNC_ASIN '2'
#define FUNC_ATAN '3'
#define FUNC_SQRT 'q'
#define FUNC_LN 'l'
#define FUNC_LOG 'o'

#define OP_PLUS '+'
#define OP_MINUS '-'
#define OP_MUL '*'
#define OP_DIV '/'
#define OP_POW '^'
#define OP_UNAR_PLUS '#'
#define OP_UNAR_MINUS '_'
#define OP_MOD '%'

#define OP_SIDE_NONE 0
#define OP_SIDE_LEFT 1
#define OP_SIDE_RIGHT 2

#define LEK_X 'x'

typedef char op_t;  // тип операторов

typedef struct node {  // лексема выражения
  op_t op;
  bool is_x;
  double num;
  struct node* next;
} Node;

typedef struct {  // список
  Node* root;
} LekList;

#define LEK_LIST_ZERO \
  (LekList) { NULL }

// ----- Notation -----
int compute_line(const char* line, double* result);

int process_notation(const char* str, LekList* list);
int compute_notation(LekList* notation, double* numbers, double x,
                     double* result);
int compute_graphic(const char* line, double* x, double* y, size_t len);

int add_func(const char** sptr, LekList* notation);
bool is_operator(char lek);
int op_rank(op_t op);
int op_side(op_t op);
int add_operator(op_t op, LekList* op_notation, LekList* out_notation,
                 bool checkUnary);
// --------------------

// ----- List Utils -----
Node* make_node(op_t op, double num, bool is_x);
int list_push(LekList* list, Node* node);
Node* list_pop(LekList* list);

void clear_list(LekList* list);
void free_node(Node* node);

bool node_is_num(Node* node);
bool node_is_op(Node* node);

bool has_x(LekList* list);

void list_copy_reversed(LekList* from, LekList* to);
// ----------------------

// ----- Utils ------
bool is_digit(char c);
bool is_alpha(char c);
bool is_space(char c);

bool cmp_func(const char* func, const char* orig);

double* alloc_numbers(LekList* notation);
// ------------------

#endif  // __NOTATION__