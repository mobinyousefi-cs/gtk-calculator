/*
 * Project:  Graphical Calculator using C and GTK+
 * File:     calculator.h
 * Author:   Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:  MIT (see LICENSE file for details)
 *
 * Description:
 *   Public interface for the calculator core. This module provides a
 *   simple expression evaluator that supports +, -, *, /, ^ and
 *   parentheses using the shunting-yard algorithm.
 */

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdbool.h>

/* Error codes describing why an evaluation failed. */

typedef enum {
    CALC_OK = 0,
    CALC_ERR_SYNTAX,
    CALC_ERR_DIV_BY_ZERO,
    CALC_ERR_UNKNOWN
} CalcErrorCode;

/* Detailed error info returned to the caller. */

typedef struct {
    CalcErrorCode code;
    char          message[128];
} CalcError;

/*
 * calc_evaluate:
 *   Evaluates the given arithmetic expression and stores the result in
 *   out_result. On success, returns true and sets out_error->code to
 *   CALC_OK. On failure, returns false and fills out_error with a
 *   human-readable message.
 */
bool calc_evaluate(const char *expr, double *out_result, CalcError *out_error);

#endif /* CALCULATOR_H */
