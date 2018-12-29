#ifndef MOLOT
#define MOLOT

typedef struct mval
{
    // dictates which kind of type this mval is
    // like MVAL_NUM or MVAL_SYM
    int type;
    // containts the value when type == MVAL_NUM
    double num;
    // err and sym both contain string data
    char *err;
    char *sym;

    int cell_count;
    struct mval **cell;
} mval; // stands for molot value

// availible types for mval.type
enum
{
    MVAL_NUM,
    MVAL_ERR,
    MVAL_SYM,
    MVAL_SEXPR,
};

mval *mval_num_new(double num);
mval *mval_err_new(char *m);
mval *mval_sym_new(char *sym);
mval *mval_sexpr_new(void);
void mval_del(mval *m);

mval *mval_read_num(mpc_ast_t *t);
mval *mval_read(mpc_ast_t *t);
mval *mval_add(mval *parent, mval *child);
void mval_print_expr(mval *m, char *open, char *close);
void mval_print(mval *m);
void mval_println(mval *v);
mval *mval_pop(mval *m, int i);
mval *mval_take(mval *m, int i);

#endif