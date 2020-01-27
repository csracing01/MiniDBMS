%{

#include <stdlib.h>
#include "y.tab.h"
struct yy_buffer_state* curr_buff();
%}

%%

("create")                                                                    { return CREATE;  }

("table")                                                                     { return TABLE; }

("char")                                                                      { return CHAR; }

("int"|"boolean"|"date")                                                      { return DATATYPE1; }

("double"|"varchar")                                                          { return DATATYPE2; }

("insert")                                                                    { return INSERT; }

("into")                                                                      { return INTO; }

("values")                                                                    { return VALUES; }

("select")                                                                    { return SELECT; }

("from")                                                                      { return FROM; }

("where")                                                                     { return WHERE; }

("delete")                                                                    { return DELETE; }

("*")                                                                         { return STAR; }

("and")                                                                       { return AND; }

("or")                                                                        { return OR; }

("not")                                                                       { return NOT; }

("<=")                                                                        { return LE; }

("<")                                                                         { return LT; }

(">=")                                                                        { return GE; }

(">")                                                                         { return GT; }

("!=")                                                                        { return NE; }

("==")                                                                        { return EQ; }

("true"|"false")                                                              { return BOOL; }

("references")                                                                { return REFERENCES; }

("foreign key")                                                               { return FORN_KEY; }

("primary key"|"not null"|"unique")                                           { return CONSTRAINT; }

(";")(" ")*("\n")                                                             { return END; }

(" ")                                                                         { }

("\n")                                                                        { }

("\t")                                                                        { }

(0[1-9]|[12][0-9]|3[01])("-")(0[1-9]|1[012])"-"[1-9][0-9][0-9][0-9]           { return DATE; }

[0-9]*"."[0-9]+                                                               { return DOUBLE; }

[0-9]+                                                                        { return DIGIT; }

[_a-zA-Z][_a-zA-Z0-9]*                                                        { return IDENTIFIER; }

("'")([a-zA-Z]+)("'")                                                         { return WORD; }

("\"")([a-zA-Z0-9]+(" ")*)*("\"")                                             { return STRING; }

("(")                                                                         { return STARTBRACKET; }

(")")                                                                         { return ENDBRACKET; }

(",")                                                                         { return COMMA; }

.                                                                             { return ELSE;  }

%%

struct yy_buffer_state* curr_buff()
{
    struct yy_buffer_state* b;
    b = yy_current_buffer;
    return b;
}
