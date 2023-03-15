#include "notation.h"

Node* make_node(op_t op, double num,
                bool is_x) {  // выделение памяти Ноды и инициализация
  Node* node = malloc(sizeof(Node));

  if (node != NULL) {
    node->op = op;
    node->num = num;
    node->is_x = is_x;
    node->next = NULL;
  }

  return node;
}

int list_push(LekList* list, Node* node) {  // добавления ноды в список
  if (node != NULL) {
    Node* tmp = list->root;
    list->root = node;
    node->next = tmp;
  }
  return node == NULL;
}

Node* list_pop(LekList* list) {  // удаление ноды из списка
  Node* res = list->root;

  if (list->root != NULL) list->root = list->root->next;

  return res;
}

bool has_x(LekList* list) {  // присутствует ли в списке x
  int res = false;

  Node* ptr = list->root;
  while (ptr != NULL && !res) {
    if (ptr->is_x) res = true;
    ptr = ptr->next;
  }
  return res;
}

bool node_is_num(Node* node) {
  return !(node->is_x) && node->op == OP_NONE;
}  // проверка ноды является ли она числом

bool node_is_op(Node* node) {
  return node->op != OP_NONE;
}  // проверка ноды, является ли оператором

void clear_list(LekList* list) {  // очистка листа и памяти
  Node* ptr = list->root;
  list->root = NULL;

  while (ptr != NULL) {
    Node* ptr_next = ptr->next;
    free(ptr);
    ptr = ptr_next;
  }
}

void free_node(Node* node) {  // очистка памяти для определенной ноды
  if (node != NULL) free(node);
}

void list_copy_reversed(LekList* from, LekList* to) {  // переворачивает слисок
  Node* node;
  while ((node = list_pop(from)) != NULL) {
    list_push(to, node);
  }
}
