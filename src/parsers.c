#include "mpc.h"

// collection of parsers; needed for cleanup
static mpc_parser_t **parsers;

mpc_parser_t *create_molot_parsers()
{
  /* Create Some Parsers */
  // ok so parsers are like pieces of lego, you use combinations of them
  // to create more interesting & complex parsers
  mpc_parser_t *Number = mpc_new("number");
  // my own addition, basically strings with [a-z] are now part of the vocab
  // mpc_parser_t *Name = mpc_new("name");

  mpc_parser_t *SymbOperator = mpc_new("symb_operator");
  mpc_parser_t *TextOperator = mpc_new("text_operator");
  mpc_parser_t *ArrOperator = mpc_new("arr_operator");
  mpc_parser_t *Symbol = mpc_new("symbol");

  mpc_parser_t *Sexpr = mpc_new("sexpr");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Molot = mpc_new("molot");
  /* Define them with the following Language */
  // these grammar/parcer definitions are regex patterns

  mpca_lang(MPCA_LANG_DEFAULT,
            "number          : /-?[0-9]+(\\.[0-9]+)?/ ;\
                symb_operator    : '+' | '-' | '*' | '/' ;\
                text_operator    : \"mul\" | \"div\" | \"sub\" | \"add\" ;\
                arr_operator     : \"min\" | \"max\" | \"avg\" | \"std\" ;\
                symbol           : <symb_operator> | <text_operator> | <arr_operator> ;\
                sexpr            : '(' <expr>* ')';\
                expr             : <number> | <symbol> | <sexpr> ;\
                molot            : /^/ <expr>* /$/  ;\
                ",
            Number,
            SymbOperator,
            TextOperator,
            ArrOperator,
            Symbol,
            Sexpr,
            Expr,
            Molot);

  // for some reason doesnt work
  // mpca_lang_contents(MPCA_LANG_DEFAULT,
  //                    "./language.molot",
  //                    Number,
  //                    SymbOperator,
  //                    TextOperator,
  //                    ArrOperator,
  //                    Operator,
  //                    Expr,
  //                    Molot);

  return Molot;
}
