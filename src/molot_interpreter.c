#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <editline/readline.h>

#include "mpc.h"
#include "mval.h"
#include "parsers.c"
#include "eval.c"

int main(int argc, char **argv)
{

    /* Print Version and Exit Information */
    puts("molot version 0.0.1; type exit or press Ctrl-C to quit");
    mpc_parser_t *Molot = create_molot_parsers();
    /* REPL */
    while (1)
    {

        /* Output our prompt and get input */
        char *input = readline("molot> ");
        if (strcmp(input, "exit") == 0)
            break;

        // user input parsing attempt
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Molot, &r))
        {
            mval *result = mval_eval(mval_read(r.output));
            mval_println(result);
            mval_del(result);

            mpc_ast_delete(r.output);

            /* Add input to history */
            add_history(input);
        }
        else
        {
            /* Otherwise Print the Error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        /* Free retrieved input */
        free(input);
    }

    return 0;
}