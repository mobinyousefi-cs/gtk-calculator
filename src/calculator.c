/*
 * Project:  Graphical Calculator using C and GTK+
 * File:     calculator.c
 * Author:   Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:  MIT (see LICENSE file for details)
 *
 * Description:
 *   Core calculator engine. Implements a small expression evaluator
 *   using the shunting-yard algorithm and a value stack. Supported
 *   operators: +, -, *, /, ^ and parentheses.
 */

#include "calculator.h"

#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 256

/* --- Utility ----------------------------------------------------------- */

static void
set_error(CalcError *err, CalcErrorCode code, const char *msg)
{
    if (!err) {
        return;
    }
    err->code = code;
    if (msg) {
        snprintf(err->message, sizeof(err->message), "%s", msg);
    } else {
        err->message[0] = '\0';
    }
}

static int
precedence(char op)
{
    switch (op) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        return -1;
    }
}

static int
is_right_associative(char op)
{
    return (op == '^');
}

static int
is_operator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

/* Apply an operator to the top two values on the value stack. */

static int
apply_operator(char op, double *val_stack, int *val_top, CalcError *err)
{
    if (*val_top < 1) {
        set_error(err, CALC_ERR_SYNTAX, "Syntax error");
        return 0;
    }

    double right = val_stack[(*val_top)--];
    double left  = val_stack[(*val_top)--];
    double res   = 0.0;

    switch (op) {
    case '+':
        res = left + right;
        break;
    case '-':
        res = left - right;
        break;
    case '*':
        res = left * right;
        break;
    case '/':
        if (right == 0.0) {
            set_error(err, CALC_ERR_DIV_BY_ZERO, "Division by zero");
            return 0;
        }
        res = left / right;
        break;
    case '^':
        res = pow(left, right);
        break;
    default:
        set_error(err, CALC_ERR_UNKNOWN, "Unknown operator");
        return 0;
    }

    val_stack[++(*val_top)] = res;
    return 1;
}

/* --- Public API -------------------------------------------------------- */

bool
calc_evaluate(const char *expr, double *out_result, CalcError *out_error)
{
    if (!expr) {
        set_error(out_error, CALC_ERR_SYNTAX, "Empty expression");
        return false;
    }

    double val_stack[MAX_STACK_SIZE];
    char   op_stack[MAX_STACK_SIZE];
    int    val_top = -1;
    int    op_top  = -1;

    set_error(out_error, CALC_OK, "");

    size_t i = 0;
    while (expr[i] != '\0') {
        char c = expr[i];

        if (isspace((unsigned char)c)) {
            ++i;
            continue;
        }

        if (isdigit((unsigned char)c) || c == '.') {
            /* Parse number using strtod */
            char *endptr = NULL;
            double value = strtod(&expr[i], &endptr);
            if (&expr[i] == endptr) {
                set_error(out_error, CALC_ERR_SYNTAX, "Invalid number");
                return false;
            }
            if (val_top >= MAX_STACK_SIZE - 1) {
                set_error(out_error, CALC_ERR_UNKNOWN, "Value stack overflow");
                return false;
            }
            val_stack[++val_top] = value;
            i = (size_t)(endptr - expr);
            continue;
        }

        if (c == '(') {
            if (op_top >= MAX_STACK_SIZE - 1) {
                set_error(out_error, CALC_ERR_UNKNOWN, "Operator stack overflow");
                return false;
            }
            op_stack[++op_top] = c;
            ++i;
            continue;
        }

        if (c == ')') {
            int found_lparen = 0;
            while (op_top >= 0) {
                char op = op_stack[op_top--];
                if (op == '(') {
                    found_lparen = 1;
                    break;
                }
                if (!apply_operator(op, val_stack, &val_top, out_error)) {
                    return false;
                }
            }
            if (!found_lparen) {
                set_error(out_error, CALC_ERR_SYNTAX, "Mismatched parentheses");
                return false;
            }
            ++i;
            continue;
        }

        if (is_operator(c)) {
            while (op_top >= 0 && is_operator(op_stack[op_top])) {
                char top_op = op_stack[op_top];
                int  prec_top = precedence(top_op);
                int  prec_cur = precedence(c);

                if (prec_top > prec_cur ||
                    (prec_top == prec_cur && !is_right_associative(c))) {
                    op_top--;
                    if (!apply_operator(top_op, val_stack, &val_top, out_error)) {
                        return false;
                    }
                } else {
                    break;
                }
            }

            if (op_top >= MAX_STACK_SIZE - 1) {
                set_error(out_error, CALC_ERR_UNKNOWN, "Operator stack overflow");
                return false;
            }

            op_stack[++op_top] = c;
            ++i;
            continue;
        }

        /* Unknown character */
        set_error(out_error, CALC_ERR_SYNTAX, "Invalid character in expression");
        return false;
    }

    /* Apply remaining operators */
    while (op_top >= 0) {
        char op = op_stack[op_top--];
        if (op == '(' || op == ')') {
            set_error(out_error, CALC_ERR_SYNTAX, "Mismatched parentheses");
            return false;
        }
        if (!apply_operator(op, val_stack, &val_top, out_error)) {
            return false;
        }
    }

    if (val_top != 0) {
        set_error(out_error, CALC_ERR_SYNTAX, "Syntax error");
        return false;
    }

    if (out_result) {
        *out_result = val_stack[val_top];
    }

    return true;
}
