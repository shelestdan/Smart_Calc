#include "notation.h"

bool is_digit(char c) { return c >= '0' && c <= '9'; }

bool is_alpha(char c) { return c >= 'a' && c <= 'z'; }

bool is_space(char c) { return c == ' '; }

bool cmp_func(const char* func, const char* orig) {
  int len = strlen(orig);
  int res = true;

  for (int i = 0; i < len && res; ++i) {
    if (func[i] != orig[i]) res = false;
  }

  return res;
}

double* alloc_numbers(LekList* notation) {  //
  double* res = NULL;
  int num_count = 0;

  Node* ptr = notation->root;
  while (ptr != NULL) {
    if (node_is_num(ptr) || ptr->is_x) num_count += 1;
    ptr = ptr->next;
  }

  if (num_count > 0) {
    res = malloc(num_count * sizeof(double));
  }

  return res;
}
