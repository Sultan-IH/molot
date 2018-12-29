#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "mpc.h"
#include "mval.h"

mval *mval_num_new(double num)
{
    mval *m = malloc(sizeof(mval));
    m->type = MVAL_NUM;

    m->num = num;
    return m;
}
mval *mval_err_new(char *err)
{
    mval *m = malloc(sizeof(mval));

    m->type = MVAL_ERR;
    m->err = malloc(strlen(err) + 1);
    strcpy(m->err, err);
    return m;
}
mval *mval_sym_new(char *sym)
{
    mval *m = malloc(sizeof(mval));
    m->type = MVAL_SYM;
    m->sym = malloc(strlen(sym) + 1); // + 1 to account for \0
    strcpy(m->sym, sym);
    return m;
}
mval *mval_sexpr_new(void)
{
    mval *m = malloc(sizeof(mval));
    m->type = MVAL_SEXPR;
    m->cell_count = 0;
    m->cell = NULL;
    return m;
}
void mval_del(mval *m)
{
    switch (m->type)
    {
    case MVAL_ERR:
        free(m->err);
        break;
    case MVAL_SYM:
        free(m->sym);
        break;
    case MVAL_SEXPR:
        for (int i = 0; i < m->cell_count; i++)
            mval_del(m->cell[i]);
        free(m->cell);
        break;
    case MVAL_NUM:
        break;
    }
    free(m);
}
mval *mval_read_num(mpc_ast_t *t)
{
    errno = 0;
    double x = strtod(t->contents, NULL);
    return errno != ERANGE ? mval_num_new(x) : mval_err_new("invalid number");
}
mval *mval_read(mpc_ast_t *t)
{
    puts("\nread result: ");
    mpc_ast_print(t);

    if (strstr(t->tag, "number"))
        return mval_read_num(t);
    if (strstr(t->tag, "symbol"))
        return mval_sym_new(t->contents);

    mval *x = NULL;
    if (strcmp(t->tag, ">") == 0)
    {
        x = mval_sexpr_new();
    }
    else if (strstr(t->tag, "sexpr"))
    {
        x = mval_sexpr_new();
    }

    for (int i = 0; i < t->children_num; i++)
    {
        if (strcmp(t->children[i]->contents, "(") == 0)
            continue;
        if (strcmp(t->children[i]->contents, ")") == 0)
            continue;
        if (strcmp(t->children[i]->tag, "regex") == 0)
            continue;
        x = mval_add(x, mval_read(t->children[i]));
    }
    return x;
}
mval *mval_add(mval *parent, mval *child)
{
    parent->cell_count++;
    // realloc() attempts to resize the memory block previously pointed to
    // by ptr* (1st arg); as a result you get more space and you don't
    // need to copy the previous data
    parent->cell = realloc(parent->cell, sizeof(mval *) * parent->cell_count);
    parent->cell[parent->cell_count - 1] = child;
    return parent;
}
void mval_print_expr(mval *m, char *open, char *close)
{
    puts(open);
    for (int i = 0; i < m->cell_count; i++)
    {
        /* Print Value contained within */
        mval_print(m->cell[i]);

        /* Don't print trailing space if last element */
        if (i != (m->cell_count - 1))
        {
            putchar(' ');
        }
    }
    puts(close);
}
void mval_print(mval *m)
{
    switch (m->type)
    {
    case MVAL_NUM:
        printf("%f", m->num);
        break;
    case MVAL_ERR:
        printf("[ERROR]: %s", m->err);
        break;
    case MVAL_SYM:
        printf("%s", m->sym);
        break;
    case MVAL_SEXPR:
        mval_print_expr(m, "(", ")");
        break;
    }
    printf("\n");
}
void mval_println(mval *v)
{
    mval_print(v);
    putchar('\n');
}

mval *mval_pop(mval *m, int i)
{
    mval *x = m->cell[i];
    memmove(&m->cell[i], &m->cell[i + 1], sizeof(mval *) * (m->cell_count - i - 1));
    m->cell_count--;
    m->cell = realloc(m->cell, sizeof(mval) * m->cell_count);
    return x;
}
mval *mval_take(mval *m, int i)
{
    mval *v = m->cell[i];
    mval_del(m);
    return v;
}