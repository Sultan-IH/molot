#include <strings.h>

#include "mpc.h"
#include "mval.h"
// declarations
mval *mval_eval_sexpr(mval *m);
mval *mval_eval(mval *v);
mval *builtin_op(mval *m, char *op);

mval *mval_eval_sexpr(mval *m)
{

    // evaulate children
    for (int i = 0; i < m->cell_count; i++)
    {
        m->cell[i] = mval_eval(m->cell[i]);
    }
    // check for errors
    for (int i = 0; i < m->cell_count; i++)
    {
        if (m->cell[i]->type == MVAL_ERR)
        {
            return mval_take(m, i);
        }
    }

    // if we have an empty expression return it
    if (m->cell_count == 0)
        return m;
    // if we have only one child we take it
    if (m->cell_count == 1)
        return mval_take(m, 1);
    // ensure first element is a symbol

    mval *op = mval_take(m, 0);
    if (op->type != MVAL_SYM)
    {
        mval_del(op);
        mval_del(m);
        return mval_err_new("first element in s-expression is not a symbol");
    }
    // evaluate the s-expression
    mval *result = builtin_op(m, op->sym);
    mval_del(op);
    return result;
}
mval *mval_eval(mval *v)
{
    puts("mval_eval called with this value: ");
    mval_print(v);
    /* Evaluate Sexpressions */
    if (v->type == MVAL_SEXPR)
    {
        return mval_eval_sexpr(v);
    }
    /* All other lval types remain the same */
    return v;
}
mval *builtin_op(mval *m, char *op)
{
    // check if all types are numbers
    for (int i = 0; i < m->cell_count; i++)
    {
        if (m->cell[i]->type != MVAL_NUM)
        {
            mval_del(m);
            return mval_err_new("cannot operate on non number types");
        }
    }
    // pop the first element

    mval *x = mval_pop(m, 0);

    /* If no arguments and sub then perform unary negation */
    if ((strcmp(op, "-") == 0) && m->cell_count == 0)
    {
        x->num = -x->num;
    }
    while (m->cell_count > 0)
    {
        mval *y = mval_pop(m, 0);
        if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0)
            x->num += y->num;
        else if ((strcmp(op, "*")) == 0 || strcmp(op, "mul") == 0)
            x->num *= y->num;
        else if ((strcmp(op, "-")) == 0 || strcmp(op, "sub") == 0)
            x->num -= y->num;
        else if ((strcmp(op, "/")) == 0 || strcmp(op, "div") == 0)
        {
            if (y->num == 0)
            {
                x = mval_err_new("tried to divide by zero");
                break;
            }
            x->num /= y->num;
        }
        else if (strcmp(op, "min") == 0)
        {
            if (x->num > y->num)
                return y;
            else
                return x;
        }
        else if (strcmp(op, "max") == 0)
        {
            if (x->num < y->num)
                return y;
            else
                return x;
        }
    }

    printf("operation invalid or not implemnted: [%s]\n", op);
    return mval_err_new("bad operation");
}