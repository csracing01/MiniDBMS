%{

#include <stdlib.h>
#include <stdio.h>
#define YYDEBUG 1

%}

%token CREATE TABLE INSERT INTO VALUES SELECT STAR FROM DELETE AND OR NOT WHERE EQ LT LE GT GE NE
%token IDENTIFIER DIGIT DOUBLE DATE BOOL ELSE END STARTBRACKET ENDBRACKET COMMA CHAR DATATYPE1 
%token DATATYPE2 CONSTRAINT FORN_KEY REFERENCES WORD STRING
//STRING
%left OR
%left AND
%left NOT
%left LT GT LE GE EQ NE

%%

program           : create table name startbracket name type 
                    | insert into table_name
                    | select columns from name whereclause conditionlist end
                    | delete from name whereclause cond end;

create            : CREATE;
  
table             : TABLE;         

name              : IDENTIFIER;     
  
startbracket      : STARTBRACKET;

type              : DATATYPE1 comma
                    | CHAR STARTBRACKET digit
                    | CHAR comma
                    | DATATYPE2 STARTBRACKET digit;

digit             : DIGIT ENDBRACKET comma;

comma             : COMMA name type
                    | constraints
                    | COMMA foreign_key
                    | endbrack;

constraints       : CONSTRAINT endbrack
                    | CONSTRAINT constraints
                    | comma;

foreign_key       : FORN_KEY STARTBRACKET IDENTIFIER ENDBRACKET REFERENCES IDENTIFIER STARTBRACKET IDENTIFIER ENDBRACKET COMMA foreign_key
                    | FORN_KEY STARTBRACKET IDENTIFIER ENDBRACKET REFERENCES IDENTIFIER STARTBRACKET IDENTIFIER ENDBRACKET endbrack;

endbrack          : ENDBRACKET end;



insert            : INSERT;

into              : INTO;

table_name        : IDENTIFIER startbracket name comma1
                    | IDENTIFIER values;

comma1            : COMMA name comma1
                    | endbracket;

endbracket        : ENDBRACKET values;

values            : VALUES startbracket val comma2;

val               : DIGIT
                    | DOUBLE
                    | DATE
                    | BOOL
                    | WORD
                    | STRING;

comma2            : COMMA val comma2
                    | endbrack;


select            : SELECT; 

columns           : star
                    | rep_columns;

rep_columns       : name
                    | rep_columns COMMA name;

star              : STAR;

from              : FROM;

whereclause       : WHERE
                    | end;

conditionlist     : name op val
                    |name op val AND conditionlist
                    |name op val OR conditionlist
                    |NOT conditionlist;

op                :  EQ
                     | LT
                     | LE
                     | GT
                     | GE 
                     | NE;



delete            : DELETE;

cond              : name op val
                    | NOT cond;

end               : END    { return 0; };

%%


int yywrap(void)
{
     return 0;
}

int yyerror(char const *s)
{
      fprintf(stderr, "%s\n", s);
      return 1;
}