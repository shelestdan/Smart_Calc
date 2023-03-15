#include "notation.h"

int compute_line(const char* line, double* result) {  // вычисление строки
  int err = line == NULL || result == NULL;

  LekList notation = LEK_LIST_ZERO;
  if (!err) err = process_notation(line, &notation);

  if (!err) {
    if (has_x(&notation)) {
      err = RES_ERR;
    } else {
      double* numbers =
          alloc_numbers(&notation);  // выделение памяти = кол-ву чисел
      err = compute_notation(&notation, numbers, NUM_ZERO, result);
      if (numbers != NULL) free(numbers);
    }
  }

  clear_list(&notation);

  return err;
}

int compute_graphic(const char* line, double* x, double* y,
                    size_t len) {  // подготовка данных для графика. Принимает
                                   // массив Х и вычисляет Y
  int err = line == NULL || x == NULL || y == NULL;

  LekList notation = LEK_LIST_ZERO;
  if (!err) err = process_notation(line, &notation);  //

  if (!err) {
    double* numbers = alloc_numbers(&notation);
    for (size_t i = 0; i < len && !err; ++i) {
      err = compute_notation(&notation, numbers, x[i], y + i);
    }

    if (numbers != NULL) free(numbers);
  }

  clear_list(&notation);

  return err;
}

int add_func(const char** sptr,
             LekList* notation) {  // добавление функции в нотацию
  char op = OP_NONE;
  const char* str = *sptr;

  if (cmp_func(str, "cos")) {
    op = FUNC_COS;
    str += 3;
  } else if (cmp_func(str, "sin")) {
    op = FUNC_SIN;
    str += 3;
  } else if (cmp_func(str, "tan")) {
    op = FUNC_TAN;
    str += 3;
  } else if (cmp_func(str, "acos")) {
    op = FUNC_ACOS;
    str += 4;
  } else if (cmp_func(str, "asin")) {
    op = FUNC_ASIN;
    str += 4;
  } else if (cmp_func(str, "atan")) {
    op = FUNC_ATAN;
    str += 4;
  } else if (cmp_func(str, "sqrt")) {
    op = FUNC_SQRT;
    str += 4;
  } else if (cmp_func(str, "ln")) {
    op = FUNC_LN;
    str += 2;
  } else if (cmp_func(str, "log")) {
    op = FUNC_LOG;
    str += 3;
  }

  *sptr = str;

  int err = op == OP_NONE;
  if (!err) err = list_push(notation, make_node(op, NUM_ZERO, false));

  return err;
}

bool is_operator(char lek) {  // является ли этот символ оператором
  int res = false;

  switch (lek) {
    case OP_PLUS:
    case OP_MINUS:
    case OP_MUL:
    case OP_DIV:
    case OP_MOD:
    case OP_POW:;
      res = true;
      break;

    default:
      res = false;
      break;
  }

  return res;
}

int op_rank(op_t op) {  // определение приоритетов
  int rank = OP_RANK_NONE;

  switch (op) {
    case OP_PLUS:
    case OP_MINUS:;
      rank = 1;
      break;

    case OP_MUL:
    case OP_DIV:
    case OP_MOD:;
      rank = 2;
      break;

    case OP_UNAR_MINUS:
    case OP_UNAR_PLUS:
      rank = 3;
      break;

    case OP_POW:
      rank = 4;
      break;

    case FUNC_SIN:
    case FUNC_COS:
    case FUNC_TAN:
    case FUNC_ASIN:
    case FUNC_ACOS:
    case FUNC_ATAN:
    case FUNC_LN:
    case FUNC_LOG:
    case FUNC_SQRT:
      rank = 5;
      break;
  }

  return rank;
}

int op_side(op_t op) {  // ассоциативные знаки
  int side = OP_SIDE_NONE;

  switch (op) {
    case OP_PLUS:
    case OP_MINUS:
    case OP_MUL:
    case OP_DIV:
    case OP_MOD:;
      side = OP_SIDE_LEFT;
      break;

    case OP_POW:;
      side = OP_SIDE_RIGHT;
      break;
  }

  return side;
}

int add_operator(op_t op, LekList* op_notation, LekList* out_notation,
                 bool checkUnary) {  // добавление оператора в ноду
  if (checkUnary) {
    if (op == OP_PLUS)
      op = OP_UNAR_PLUS;
    else if (op == OP_MINUS)
      op = OP_UNAR_MINUS;
  }

  Node* lek;
  while (
      (lek = list_pop(op_notation)) != NULL && lek->op != OP_NONE &&
      (op_rank(lek->op) > op_rank(op) ||
       (op_side(lek->op) == OP_SIDE_LEFT && op_rank(lek->op) == op_rank(op)))) {
    list_push(out_notation, lek);
  }

  if (lek != NULL) list_push(op_notation, lek);

  int err = list_push(op_notation, make_node(op, NUM_ZERO, false));
  return err;
}

int process_notation(const char* str,
                     LekList* notation) {  // функция перевода строки в нотацию
  int err = RES_OK;

  LekList out_notation = LEK_LIST_ZERO;
  LekList op_notation = LEK_LIST_ZERO;

  const char* sptr = str;
  char sc = '\0';

  bool checkUnary = true;
  bool canBeNumber = true;
  bool nextOnlyBkt = false;

  while ((sc = *sptr) != '\0' && !err) {
    if (!is_space(sc) && nextOnlyBkt && sc != BKT_OPEN) {
      err = RES_ERR;
      continue;
    }

    if (is_digit(sc) && canBeNumber) {
      double num = 0.0;
      int inc = 0;
      sscanf(sptr, "%lf%n", &num, &inc);
      if (inc > 0) sptr += inc - 1;

      err = list_push(&out_notation, make_node(OP_NONE, num, false));
      checkUnary = false;
      canBeNumber = false;
    }

    else if (sc == LEK_X && canBeNumber) {
      err = list_push(&out_notation, make_node(OP_NONE, NUM_ZERO, true));
      checkUnary = false;
      canBeNumber = false;
    }

    else if (is_alpha(sc)) {
      err = add_func(&sptr, &op_notation);
      if (!err) sptr -= 1;
      nextOnlyBkt = true;
      canBeNumber = false;
      checkUnary = false;
    }

    else if (is_operator(sc)) {
      err = add_operator(sc, &op_notation, &out_notation, checkUnary);
      checkUnary = true;
      canBeNumber = true;
    }

    else if (sc == BKT_OPEN) {
      err = list_push(&op_notation, make_node(BKT_OPEN, NUM_ZERO, false));
      checkUnary = true;
      nextOnlyBkt = false;
      canBeNumber = true;
    }

    else if (sc == BKT_CLOSE) {
      Node* lek;
      while ((lek = list_pop(&op_notation)) != NULL && lek->op != BKT_OPEN) {
        err = list_push(&out_notation, lek);
      }

      if (lek == NULL) {
        err = RES_ERR;
      } else {
        free_node(lek);
      }

      checkUnary = false;
      canBeNumber = false;
    }

    else if (!is_space(sc)) {
      err = RES_ERR;
    }

    sptr += 1;
  }

  Node* lek;
  while (!err && (lek = list_pop(&op_notation)) != NULL) {
    if (lek->op == BKT_OPEN) {
      err = RES_ERR;
      free_node(lek);
    } else {
      list_push(&out_notation, lek);
    }
  }

  clear_list(&op_notation);

  if (err) {
    clear_list(&out_notation);
  } else {
    list_copy_reversed(&out_notation, notation);
  }

  return err;
}

int compute_notation(LekList* notation, double* numbers, double x,
                     double* result) {  // вычисление нотации
  int err = numbers == NULL || notation == NULL;

  int nums_len = 0;

  Node* node = NULL;

  if (!err) {
    node = notation->root;
  }

  while (node != NULL && !err) {
    if (node_is_num(node)) {
      numbers[nums_len] = node->num;
      nums_len += 1;
    } else if (node->is_x) {
      numbers[nums_len] = x;
      nums_len += 1;
    } else if (node_is_op(node)) {
      if (node->op == OP_PLUS && nums_len >= 2) {
        numbers[nums_len - 2] = numbers[nums_len - 2] + numbers[nums_len - 1];
        nums_len -= 1;
      } else if (node->op == OP_MINUS && nums_len >= 2) {
        numbers[nums_len - 2] = numbers[nums_len - 2] - numbers[nums_len - 1];
        nums_len -= 1;
      } else if (node->op == OP_MUL && nums_len >= 2) {
        numbers[nums_len - 2] = numbers[nums_len - 2] * numbers[nums_len - 1];
        nums_len -= 1;
      } else if (node->op == OP_DIV && nums_len >= 2) {
        numbers[nums_len - 2] = numbers[nums_len - 2] / numbers[nums_len - 1];
        nums_len -= 1;
      } else if (node->op == OP_MOD && nums_len >= 2) {
        numbers[nums_len - 2] =
            fmod(numbers[nums_len - 2], numbers[nums_len - 1]);
        nums_len -= 1;
      } else if (node->op == OP_POW && nums_len >= 2) {
        numbers[nums_len - 2] =
            pow(numbers[nums_len - 2], numbers[nums_len - 1]);
        nums_len -= 1;
      } else if (node->op == OP_UNAR_MINUS && nums_len >= 1) {
        numbers[nums_len - 1] *= -1;
      } else if (node->op == FUNC_SIN && nums_len >= 1) {
        numbers[nums_len - 1] = sin(numbers[nums_len - 1]);
      } else if (node->op == FUNC_COS && nums_len >= 1) {
        numbers[nums_len - 1] = cos(numbers[nums_len - 1]);
      } else if (node->op == FUNC_TAN && nums_len >= 1) {
        numbers[nums_len - 1] = tan(numbers[nums_len - 1]);
      } else if (node->op == FUNC_ASIN && nums_len >= 1) {
        numbers[nums_len - 1] = asin(numbers[nums_len - 1]);
      } else if (node->op == FUNC_ACOS && nums_len >= 1) {
        numbers[nums_len - 1] = acos(numbers[nums_len - 1]);
      } else if (node->op == FUNC_ATAN && nums_len >= 1) {
        numbers[nums_len - 1] = atan(numbers[nums_len - 1]);
      } else if (node->op == FUNC_SQRT && nums_len >= 1) {
        numbers[nums_len - 1] = sqrt(numbers[nums_len - 1]);
      } else if (node->op == FUNC_LN && nums_len >= 1) {
        numbers[nums_len - 1] = log(numbers[nums_len - 1]);
      } else if (node->op == FUNC_LOG && nums_len >= 1) {
        numbers[nums_len - 1] = log10(numbers[nums_len - 1]);
      } else if (node->op != OP_UNAR_PLUS || nums_len < 1) {
        err = RES_ERR;
      }
    }
    node = node->next;
  }

  if (nums_len > 1) {
    err = RES_ERR;
  }

  if (!err) {
    *result = numbers[0];
  } else if (result != NULL) {
    *result = 0.0;
  }

  return err;
}
