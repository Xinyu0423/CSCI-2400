
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     VAR = 258,
     INT = 259,
     FLOAT = 260,
     VOID = 261,
     UNSIGNED = 262,
     LONG = 263,
     SHORT = 264,
     CHAR = 265,
     NUM = 266,
     SEMI = 267,
     COMMA = 268,
     LPAREN = 269,
     RPAREN = 270,
     LBRACE = 271,
     RBRACE = 272,
     LBRACK = 273,
     RBRACK = 274,
     ASSIGN = 275,
     CARATASSIGN = 276,
     AMPASSIGN = 277,
     STARASSIGN = 278,
     PLUSASSIGN = 279,
     MINUSASSIGN = 280,
     BARASSIGN = 281,
     LEFTSHIFTASSIGN = 282,
     RIGHTSHIFTASSIGN = 283,
     SLASHASSIGN = 284,
     PERCENTASSIGN = 285,
     PLUSPLUS = 286,
     MINUSMINUS = 287,
     AMPAMP = 288,
     BARBAR = 289,
     TILDE = 290,
     BANG = 291,
     CARAT = 292,
     AMP = 293,
     BAR = 294,
     STAR = 295,
     PLUS = 296,
     MINUS = 297,
     SLASH = 298,
     PERCENT = 299,
     LESSLESS = 300,
     GREATERGREATER = 301,
     COLON = 302,
     QUESTION = 303,
     NOTEQUAL = 304,
     EQUAL = 305,
     LESS = 306,
     LESSEQUAL = 307,
     GREATER = 308,
     GREATEREQUAL = 309,
     RETURN = 310,
     IF = 311,
     ELSE = 312,
     WHILE = 313,
     DO = 314,
     FOR = 315,
     BREAK = 316,
     CONTINUE = 317,
     CASE = 318,
     SWITCH = 319,
     DEFAULT = 320,
     SIZEOF = 321
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


