
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "simplec.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "gen-hash.h"
#include "boolnet.h"
#include "ast.h"
#define YYSTYPE node_ptr

#define DEBUG 0

/* Current line number.  Maintained by lex */
int lineno = 1;
#define ERRLIM 0

int errlim = ERRLIM;
int timelim = 0; /* Max number of seconds for BDD checker (0 means infinity) */

int errcnt = 0;

FILE *outfile = NULL;
FILE *infofile = NULL;
extern FILE *yyin;

char *infilename = NULL;
/* Optional pattern giving type information about arguments */
char *argpattern = NULL;
char *function_name = NULL;
node_ptr function_body = NULL;
node_ptr return_type = NULL;
/* Current type of declared variables */
node_ptr default_type = NULL;

int yyparse(void);
int yylex(void);

/* Utility functions */
/* Handle timeouts */
void timeout_handler(int sig)
{
  fprintf(infofile, "Timeout: Checking exceeded %d seconds\n", timelim);
  exit(0);
}

/* Finishing message */
 void finish()
 {
   exit(0);
 } 

void yyerror(const char *str)
{
  fprintf(stdout, "Error, %s:%d: %s\n",
	  infilename ? infilename : "", lineno, str);
  if (++errcnt > errlim) {
      fprintf(stdout, "Too many errors, aborting\n");
      exit(1);
  }
  exit(1);
}

static char errmsg[1024];
void yyserror(const char *str, char *other)
{
    sprintf(errmsg, str, other);
    yyerror(errmsg);
}

int yywrap()
{
  return 1;
}

/* Count number of newlines in string */  
int count_returns(char *s) {
  int result = 0;
  int c = 0;
  while ((c = *s++) != '\0')
    result += (c == '\n');
  return result;
}

static void usage(char *name)
{
  fprintf(stdout, "Usage: %s [-r][-s][-a argpattern] f1.c f2.c ... [-o out.cnf][-e errlim][-u unrolllim] [-t timelim]\n", name);
  fprintf(stdout, " -r: Only test for runtime errors (uninitialized vars., etc.)\n");  
  if  (have_bdds)
    fprintf(stdout, " -s: Set up SAT problem, rather than using BDDs\n");
  fprintf(stdout, " -u lim: Limit maximum number of times loop is repeated\n");
  fprintf(stdout, " -t secs: Limit number of seconds for BDD checking\n");
  fprintf(stdout, " -a argpattern: Specify argument ranges\n");
  fprintf(stdout, " -C: Disable casting (implicit & explicit) in first code file\n");
  fprintf(stdout, " -d dumpfile: Dump counterexample BDD\n");
  fprintf(stdout, "Argpattern of form pat1:pat2:...:patn\n");
  fprintf(stdout, "Possible patterns:\n");
  fprintf(stdout, "  Default: Same as t%d\n", LSIZE);
  fprintf(stdout, "  tK: K-bit, two's complement representation\n");
  fprintf(stdout, "  uK: K-bit, unsigned representation\n");
  fprintf(stdout, "  [lower,upper]: Give specific value range\n");
  fprintf(stdout, " -o outfile: Specify output CNF file\n");
  exit(0);
}

int main(int argc, char **argv)
{
    /* When have single function, Check whether always returns 1 */
    bvec_ptr ref_rval = int_const(1); 
    bvec_ptr new_rval;
    /* AST for function */
    node_ptr fnode;
    /* Do all functions satisfy runtime constraints? */
    op_ptr all_runtime = one();
    /* What are the conditions for out of range arguments */
    op_ptr bad_args = zero();
    /* Do all of the function results match? */
    op_ptr all_rm = one();
    /* Latest computation of runtime and results match */
    op_ptr runtime;
    op_ptr rm;
    eval_status_ele status;
    int runtime_only = 0;
    int cnt = 0;
    int use_bdds = have_bdds;
    int cast_ok = 1;
    int i;
    int unroll_limit = 33;
    int found_runtime_error = 0;
    char *dumpfilename = NULL;

    clock_t start_time = clock();


    argpattern = NULL;
    outfile = stdout;
    infofile = stdout;

    default_type = cast_node(ISIZE, DATA_SIGNED, NULL);
    return_type = cast_node(ISIZE, DATA_SIGNED, NULL);

    /* Make two passes through arguments.  First, pick up all options */
    for (i = 1; i < argc; i++) {
	if (argv[i][0] != '-')
	  continue;
	/* Parse options */
	switch (argv[i][1]) {
	case 'h': usage(argv[0]);
	  break;
	case 'r': runtime_only = 1;
	  break;
	case 'C': cast_ok = 0;
	  break;
        case 'u': unroll_limit = atoi(argv[++i]);
          break;
        case 't': timelim = atoi(argv[++i]);
          break;
	case 'd': dumpfilename = argv[++i];
	  break;
	case 'o': 
	  outfile = fopen (argv[++i], "w");
	  if (!outfile) {
	    yyserror("Couldn't open output file '%s'", argv[i]);
	    exit(1);
	  }
	  break;
	case 'a':
	  argpattern = argv[++i];
	  break;
	case 'e':
	  errlim = atoi(argv[++i]);
	  break;
	case 's':
	  /* Only allow this argument when BDDs are a possibility */
	  if (have_bdds) {
	    use_bdds = 0;
	    break;
	  }
	default:
	  usage(argv[0]);
	  break;
	}
    }
    /* Set up signal handler when using BDDs */
    if (timelim > 0) {
      signal(SIGALRM, timeout_handler);
    }
    /* Second pass.  Process C files */
    init_ast_gen();
    init_ast_eval(argpattern);
    
    for (i = 1; i < argc; i++) {
      if (*argv[i] == '-') {
	char opt = argv[i][1];
	if (opt == 'a' || opt == 'o' || opt == 'e' || opt == 'u' || opt == 't' || opt == 'd')
	  i++;
	continue;
      }
      infilename = strsave(argv[i]);
      yyin = fopen(infilename, "r");
      if (!yyin) {
	yyserror("Couldn't open file '%s'", infilename);
	exit(1);
      }
      init_ast_gen();
      lineno = 1;
      if (yyparse())
	exit(1);
      new_rval = eval_ast(function_body, return_type, unroll_limit, &status);
      if (cnt == 0 && !cast_ok && status.casting > 0) {
	fprintf(infofile, "Disallowed casting in file %s\n", infilename);
      }
      runtime = status.all_ok;
      bad_args = status.bad_args;
      if (runtime != one()) {
	  if (use_bdds) {
	      if (timelim > 0) {
	          alarm(timelim);
	      }
	      if (gen_solve(NULL, not_op(runtime))) {
		  fprintf(infofile, "Runtime error(s) in file %s\n", infilename);
		  found_runtime_error = 1;
		  if (timelim > 0) alarm(timelim);
		  if (gen_solve(NULL, status.incomplete_loop)) {
		      fprintf(infofile, "Loop failed to terminate within %d iterations: ",
			      unroll_limit);
		      gen_solve(infofile, status.incomplete_loop);
		      finish();
		  }
		  if (timelim > 0) alarm(timelim);
		  if (gen_solve(NULL, status.uninitialized_variable)) {
		      fprintf(infofile, "Variable used before being initialized: ");
		      gen_solve(infofile, status.uninitialized_variable);
		      finish();
		  }
		  if (timelim > 0) alarm(timelim);
		  if (gen_solve(NULL, status.missing_return)) {
		      fprintf(infofile, "No return executed: ");
		      gen_solve(infofile, status.missing_return);
		      finish();
		  }
		  if (timelim > 0) alarm(timelim);
		  if (gen_solve(NULL, status.uncaught_break)) {
		      fprintf(infofile, "Uncaught break statement: ");
		      gen_solve(infofile, status.uncaught_break);
		      finish();
		  }
		  if (timelim > 0) alarm(timelim);
		  if (gen_solve(NULL, status.uncaught_continue)) {
		      fprintf(infofile, "Uncaught continue statement: ");
		      gen_solve(infofile, status.uncaught_continue);
		      finish();
		  }
		  if (timelim > 0) alarm(timelim);
		  if (gen_solve(NULL, status.mem_error)) {
		      fprintf(infofile, "Memory or array referencing error: ");
		      gen_solve(infofile, status.mem_error);
		      finish();
		  }
		  if (timelim > 0) alarm(timelim);
		  if (gen_solve(NULL, status.div_error)) {
		      fprintf(infofile, "Zero divide error: ");
		      gen_solve(infofile, status.div_error);
		      finish();
		  }
		  if (timelim > 0) alarm(timelim);
		  if (gen_solve(NULL, status.shift_error)) {
		      fprintf(infofile, "Invalid shift error: ");
		      gen_solve(infofile, status.shift_error);
		      finish();
		  }
	      }
	  } else {
	      fprintf(infofile, "Warning: possible runtime error in file %s\n",
		infilename);
	      if (status.incomplete_loop != zero())
		  fprintf(infofile,
			  "  Loop possibly did not terminate within %d iterations\n", unroll_limit);
	      if (status.uninitialized_variable != zero())
		  fprintf(infofile,
			  "  Possibly used variable that had not been initialized\n");
	      if (status.missing_return != zero())
		  fprintf(infofile,
			  "  Possibly failed to execute return statement\n");
	      if (status.uncaught_break != zero())
		  fprintf(infofile,
			  "  Possibly failed to catch break statement\n");
	      if (status.uncaught_continue != zero())
		  fprintf(infofile,
			  "  Possibly failed to catch continue statement\n");
	      if (status.mem_error != zero())
		  fprintf(infofile,
			  "  Possible memory or array referencing error\n");
	      if (status.div_error != zero())
		  fprintf(infofile,
			  "  Possible zero divide error\n");
	      if (status.shift_error != zero())
		  fprintf(infofile,
			  "  Possible shift error\n");
	  }
      }

      all_runtime = and_op(runtime, all_runtime);
      cnt++;
      if (!runtime_only) {
	rm = or_op(bad_args, int_eq(ref_rval, new_rval));
	if (cnt == 1) {
	  /* Save this as reference value
	     in case do further comparisons */
	  ref_rval = new_rval;
	}
	if (cnt == 2) {
	  /* Special case of switching from single predicate function
	     to comparing multiple functions */
	  all_rm = rm;
	} else
	  all_rm = and_op(all_rm, rm);
      }
    }

    if (use_bdds) {
      op_ptr check;
      if (!found_runtime_error) {
	fprintf(infofile, "Bug Condition ");
	if (timelim > 0) {
	  alarm(timelim);
	}
	check = not_op(and_op(all_rm, all_runtime));
	gen_solve(infofile, check);
	if (dumpfilename != NULL) {
	  /* Dump vector of nonzero functions */
	  int dump_cnt = ISIZE;
	  int i;
	  while (dump_cnt > 1 && ref_rval->bits[dump_cnt-1] == zero())
	    dump_cnt--;
	  FILE *fp = fopen(dumpfilename, "w");
	  op_ptr *funct_set = calloc(sizeof(op_ptr), dump_cnt);
	  char **funct_names = calloc(sizeof(char *), dump_cnt);
	  for (i = 0; i < dump_cnt; i++) {
	    char buf[16];
	    funct_set[i] = ref_rval->bits[i];
	    sprintf(buf, "F_%d", i);
	    funct_names[i] = strsave(buf);
	  }
	  if (!fp) {
	    fprintf(stderr, "Couldn't open dump file '%s'\n", dumpfilename);
	    exit(1);
	  }
	  dump_blif(fp, dump_cnt, funct_set, funct_names);
	  fclose(fp);
	}
      }
    }
    else
      gen_cnf(outfile, infofile, not_op(and_op(all_rm, all_runtime)));
    fprintf(infofile, "Time: %.2f sec.\n",
	    (clock() - start_time)/(double) CLOCKS_PER_SEC);
    finish();
    return 0;
}

int old_main(int argc, char **argv)
{
  int i;
  init_ast_eval(NULL);
  init_ast_gen();
  default_type = cast_node(ISIZE, DATA_SIGNED, NULL);
  return_type = cast_node(ISIZE, DATA_SIGNED, NULL);
  for (i = 1; i < argc; i++) {
    infilename = strsave(argv[i]);
    yyin = fopen(infilename, "r");
    if (!yyin) {
      yyserror("Couldn't open file '%s'", infilename);
      exit(1);
    }
    lineno = 1;
    if (yyparse())
      exit(1);
    printf("Function %s:\n", function_name);
    show_node(stdout, function_body, 1);
  }
}



/* Line 189 of yacc.c  */
#line 459 "simplec.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 567 "simplec.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  19
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   652

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  116
/* YYNRULES -- Number of states.  */
#define YYNSTATES  226

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   321

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,    12,    14,    17,    20,    24,    28,    32,
      36,    39,    42,    45,    48,    51,    55,    59,    64,    69,
      72,    74,    76,    78,    81,    83,    87,    90,    92,    95,
      96,    98,   101,   106,   107,   110,   114,   117,   119,   123,
     126,   129,   135,   143,   153,   161,   167,   171,   177,   182,
     186,   188,   190,   194,   198,   200,   205,   209,   213,   217,
     219,   224,   226,   228,   232,   237,   240,   243,   246,   251,
     254,   257,   260,   263,   266,   269,   274,   276,   280,   284,
     288,   292,   296,   300,   304,   308,   312,   316,   320,   324,
     328,   332,   336,   340,   344,   348,   350,   356,   358,   362,
     366,   370,   374,   378,   382,   386,   390,   394,   398,   402,
     404,   408,   409,   411,   413,   417,   419
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      68,     0,    -1,    69,     3,    14,    70,    15,    16,    71,
      17,    -1,     7,    -1,     7,     8,    -1,     8,     7,    -1,
       7,     8,     8,    -1,     8,     8,     7,    -1,     7,     9,
       4,    -1,     9,     7,     4,    -1,     7,     9,    -1,     9,
       7,    -1,     7,    10,    -1,    10,     7,    -1,     7,     4,
      -1,     8,     7,     4,    -1,     7,     8,     4,    -1,     8,
       8,     7,     4,    -1,     7,     8,     8,     4,    -1,     9,
       4,    -1,     9,    -1,    10,    -1,     4,    -1,     8,     4,
      -1,     8,    -1,     8,     8,     4,    -1,     8,     8,    -1,
       5,    -1,    69,    40,    -1,    -1,     6,    -1,    69,     3,
      -1,    70,    13,    69,     3,    -1,    -1,    71,    72,    -1,
      69,    73,    12,    -1,    81,    12,    -1,    12,    -1,    55,
      81,    12,    -1,    61,    12,    -1,    62,    12,    -1,    58,
      14,    81,    15,    72,    -1,    59,    72,    58,    14,    81,
      15,    12,    -1,    60,    14,    82,    12,    82,    12,    82,
      15,    72,    -1,    56,    14,    81,    15,    72,    57,    72,
      -1,    56,    14,    81,    15,    72,    -1,    16,    71,    17,
      -1,    64,    14,    81,    15,    72,    -1,    63,    79,    47,
      72,    -1,    65,    47,    72,    -1,    74,    -1,    75,    -1,
      73,    13,    74,    -1,    73,    13,    75,    -1,     3,    -1,
      74,    18,    78,    19,    -1,    74,    18,    19,    -1,    74,
      20,    80,    -1,    74,    20,    83,    -1,     3,    -1,    76,
      18,    78,    19,    -1,    76,    -1,    11,    -1,    14,    81,
      15,    -1,    14,    69,    15,    77,    -1,    36,    77,    -1,
      35,    77,    -1,    42,    77,    -1,    66,    14,    69,    15,
      -1,    76,    31,    -1,    76,    32,    -1,    32,    76,    -1,
      31,    76,    -1,    38,    76,    -1,    40,    76,    -1,     3,
      14,    81,    15,    -1,    77,    -1,    78,    40,    78,    -1,
      78,    41,    78,    -1,    78,    42,    78,    -1,    78,    43,
      78,    -1,    78,    44,    78,    -1,    78,    45,    78,    -1,
      78,    46,    78,    -1,    78,    51,    78,    -1,    78,    52,
      78,    -1,    78,    53,    78,    -1,    78,    54,    78,    -1,
      78,    50,    78,    -1,    78,    49,    78,    -1,    78,    38,
      78,    -1,    78,    37,    78,    -1,    78,    39,    78,    -1,
      78,    33,    78,    -1,    78,    34,    78,    -1,    78,    -1,
      78,    48,    79,    47,    79,    -1,    79,    -1,    76,    20,
      80,    -1,    76,    21,    80,    -1,    76,    22,    80,    -1,
      76,    23,    80,    -1,    76,    24,    80,    -1,    76,    29,
      80,    -1,    76,    30,    80,    -1,    76,    25,    80,    -1,
      76,    26,    80,    -1,    76,    27,    80,    -1,    76,    28,
      80,    -1,    80,    -1,    81,    13,    80,    -1,    -1,    81,
      -1,    80,    -1,    16,    84,    17,    -1,    83,    -1,    83,
      13,    84,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   411,   411,   417,   418,   419,   420,   421,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   445,
     446,   447,   448,   451,   452,   456,   457,   458,   459,   460,
     461,   462,   465,   470,   475,   476,   477,   478,   480,   481,
     485,   486,   487,   488,   492,   493,   494,   498,   500,   505,
     506,   510,   511,   512,   513,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   548,   549,   556,   557,   561,   562,   563,
     565,   567,   569,   571,   573,   575,   577,   582,   584,   590,
     591,   596,   597,   602,   603,   607,   608
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "VAR", "INT", "FLOAT", "VOID",
  "UNSIGNED", "LONG", "SHORT", "CHAR", "NUM", "SEMI", "COMMA", "LPAREN",
  "RPAREN", "LBRACE", "RBRACE", "LBRACK", "RBRACK", "ASSIGN",
  "CARATASSIGN", "AMPASSIGN", "STARASSIGN", "PLUSASSIGN", "MINUSASSIGN",
  "BARASSIGN", "LEFTSHIFTASSIGN", "RIGHTSHIFTASSIGN", "SLASHASSIGN",
  "PERCENTASSIGN", "PLUSPLUS", "MINUSMINUS", "AMPAMP", "BARBAR", "TILDE",
  "BANG", "CARAT", "AMP", "BAR", "STAR", "PLUS", "MINUS", "SLASH",
  "PERCENT", "LESSLESS", "GREATERGREATER", "COLON", "QUESTION", "NOTEQUAL",
  "EQUAL", "LESS", "LESSEQUAL", "GREATER", "GREATEREQUAL", "RETURN", "IF",
  "ELSE", "WHILE", "DO", "FOR", "BREAK", "CONTINUE", "CASE", "SWITCH",
  "DEFAULT", "SIZEOF", "$accept", "funct", "type", "arglist", "statements",
  "statement", "decllist", "decl", "adecl", "vexpr", "uexpr", "expr",
  "qexpr", "aexpr", "cexpr", "ocexpr", "iexpr", "iexprlist", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    67,    68,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    70,
      70,    70,    70,    71,    71,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      73,    73,    73,    73,    74,    74,    74,    75,    75,    76,
      76,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    79,    79,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    81,
      81,    82,    82,    83,    83,    84,    84
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     8,     1,     2,     2,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     3,     3,     4,     4,     2,
       1,     1,     1,     2,     1,     3,     2,     1,     2,     0,
       1,     2,     4,     0,     2,     3,     2,     1,     3,     2,
       2,     5,     7,     9,     7,     5,     3,     5,     4,     3,
       1,     1,     3,     3,     1,     4,     3,     3,     3,     1,
       4,     1,     1,     3,     4,     2,     2,     2,     4,     2,
       2,     2,     2,     2,     2,     4,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     0,     1,     1,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    22,    27,     3,    24,    20,    21,     0,     0,    14,
       4,    10,    12,    23,     5,    26,    19,    11,    13,     1,
       0,    28,    16,     6,     8,    15,    25,     7,     9,    29,
      18,    17,    30,     0,     0,    31,     0,     0,     0,    33,
      32,     0,    59,    62,    37,     0,    33,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,    61,    76,
      95,    97,   109,     0,     0,     0,     0,     0,    59,    72,
      71,    61,    66,    65,    73,    74,    67,     0,     0,     0,
       0,   111,    39,    40,     0,     0,     0,     0,    54,     0,
      50,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,     0,     0,     0,    63,
      46,    38,     0,     0,     0,   112,     0,     0,     0,    49,
       0,    35,     0,     0,     0,     0,    98,    99,   100,   101,
     102,   105,   106,   107,   108,   103,   104,    93,    94,    91,
      90,    92,    77,    78,    79,    80,    81,    82,    83,     0,
      89,    88,    84,    85,    86,    87,   110,    75,    64,     0,
       0,     0,   111,    48,     0,    68,    52,    53,    56,     0,
       0,    57,    58,    60,     0,    45,    41,     0,     0,    47,
      55,   113,   115,     0,    96,     0,     0,   111,     0,   114,
      44,    42,     0,   116,     0,    43
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,    66,    34,    41,    67,    99,   100,   101,    68,
      69,    70,    71,    72,    73,   146,   212,   213
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -165
static const yytype_int16 yypact[] =
{
     118,  -165,  -165,    17,    10,    50,    23,    40,     2,  -165,
      62,   120,  -165,  -165,   126,    82,  -165,   130,  -165,  -165,
      33,  -165,  -165,   131,  -165,  -165,  -165,   153,  -165,    55,
    -165,  -165,  -165,     8,   105,  -165,   118,   113,     9,  -165,
    -165,   181,   147,  -165,  -165,   349,  -165,  -165,   142,   142,
     128,   128,   142,   142,   128,   128,   148,   151,   309,   182,
     191,   192,   128,   193,   158,   194,    12,  -165,    51,  -165,
     515,  -165,  -165,    -4,   128,    -2,   125,   245,  -165,   196,
     196,   -12,  -165,  -165,   196,   196,  -165,    11,   128,   128,
     152,   128,  -165,  -165,   162,   128,   309,   118,  -165,    43,
     123,  -165,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,  -165,  -165,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,  -165,   128,   140,   128,  -165,
    -165,  -165,   141,   156,   197,   202,   206,   309,   186,  -165,
       1,  -165,   217,   385,   397,   465,  -165,  -165,  -165,  -165,
    -165,  -165,  -165,  -165,  -165,  -165,  -165,   533,   403,   568,
     583,   551,  -165,    44,    44,  -165,  -165,   108,   108,   175,
     598,   598,    52,    52,    52,    52,  -165,  -165,  -165,   309,
     309,   128,   128,  -165,   309,  -165,   123,  -165,  -165,   493,
     397,  -165,  -165,  -165,   128,   168,  -165,   187,   214,  -165,
    -165,  -165,   215,   210,  -165,   309,   218,   128,   397,  -165,
    -165,  -165,   216,  -165,   309,  -165
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -165,  -165,    22,  -165,   183,   -57,  -165,    80,    81,   -17,
     -47,   348,   -60,    70,   -45,  -164,    84,    16
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      76,    90,    94,    82,    83,    20,   102,    86,   135,   136,
      87,    35,    40,   138,    13,    98,   195,    14,    15,   114,
     115,     9,     8,   141,   136,    10,    11,    12,   208,   137,
      18,    79,    80,    81,    81,    84,    85,    81,    21,   149,
      19,    21,    21,   142,   143,    81,   145,    29,    21,    21,
     148,    33,    21,   222,    16,   151,   152,    17,    38,     1,
       2,    32,     3,     4,     5,     6,    22,    75,   179,   102,
      23,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   121,    81,    26,   124,   125,    27,
     193,   188,   121,   122,   123,   124,   125,   126,   127,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    36,   150,
      37,    81,     1,     2,    24,     3,     4,     5,     6,    39,
      25,    42,   205,   206,    28,    30,    81,   209,   136,    43,
     139,   153,    45,   154,   214,    78,   207,   145,   121,   122,
     123,   124,   125,   136,   136,   187,   189,    31,   220,    48,
      49,    74,    88,    50,    51,    89,    52,   225,    53,   136,
      54,   190,   145,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    42,     1,     2,    81,     3,     4,
       5,     6,    43,    44,    65,    45,    91,    46,    47,   136,
     136,   194,   216,    92,    93,    96,   186,    95,    97,   147,
     144,   191,    48,    49,   102,   136,    50,    51,   192,    52,
      98,    53,   204,    54,   201,   215,   217,   219,   218,    77,
     221,   224,   196,   197,   223,     0,    55,    56,   202,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    42,     1,
       2,     0,     3,     4,     5,     6,    43,    44,     0,    45,
       0,    46,   140,     0,     0,     0,     0,     0,     0,     0,
     211,     0,     0,     0,     0,     0,    48,    49,     0,     0,
      50,    51,     0,    52,     0,    53,     0,    54,   211,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,    56,     0,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    42,     1,     2,     0,     3,     4,     5,     6,
      43,    44,     0,    45,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,     0,     0,    50,    51,     0,    52,     0,    53,
       0,    54,    42,     1,     2,     0,     3,     4,     5,     6,
      43,     0,     0,    45,    55,    56,     0,    57,    58,    59,
      60,    61,    62,    63,    64,    65,     0,     0,     0,     0,
      48,    49,     0,     0,    50,    51,     0,    52,    42,    53,
       0,    54,     0,     0,     0,     0,    43,     0,     0,    45,
      42,     0,     0,     0,   198,     0,     0,     0,    43,     0,
       0,    45,     0,   200,     0,    65,    48,    49,     0,     0,
      50,    51,     0,    52,     0,    53,     0,    54,    48,    49,
       0,     0,    50,    51,     0,    52,   116,    53,     0,    54,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     155,    65,   129,   130,   131,   132,   133,   134,     0,     0,
       0,     0,     0,    65,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,     0,   180,   181,   182,
     183,   184,   185,     0,   203,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
       0,   199,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   210,     0,   129,   130,   131,   132,   133,   134,
       0,     0,     0,     0,     0,     0,   116,   117,     0,     0,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
       0,     0,   129,   130,   131,   132,   133,   134,   116,   117,
       0,     0,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,     0,   128,   129,   130,   131,   132,   133,   134,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
       0,     0,   129,   130,   131,   132,   133,   134,   118,   119,
       0,   121,   122,   123,   124,   125,   126,   127,     0,     0,
     129,   130,   131,   132,   133,   134,   119,     0,   121,   122,
     123,   124,   125,   126,   127,     0,     0,   129,   130,   131,
     132,   133,   134,   121,   122,   123,   124,   125,   126,   127,
       0,     0,   129,   130,   131,   132,   133,   134,   121,   122,
     123,   124,   125,   126,   127,     0,     0,     0,     0,   131,
     132,   133,   134
};

static const yytype_int16 yycheck[] =
{
      45,    58,    62,    50,    51,     3,    18,    54,    12,    13,
      55,     3,     3,    15,     4,     3,    15,     7,     8,    31,
      32,     4,     0,    12,    13,     8,     9,    10,   192,    74,
       7,    48,    49,    50,    51,    52,    53,    54,    40,    96,
       0,    40,    40,    88,    89,    62,    91,    14,    40,    40,
      95,    29,    40,   217,     4,    12,    13,     7,    36,     4,
       5,     6,     7,     8,     9,    10,     4,    45,   128,    18,
       8,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    40,   102,     4,    43,    44,     7,
     147,   138,    40,    41,    42,    43,    44,    45,    46,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    13,    97,
      15,   138,     4,     5,     4,     7,     8,     9,    10,    16,
       4,     3,   189,   190,     4,     4,   153,   194,    13,    11,
      15,    18,    14,    20,   204,     3,   191,   192,    40,    41,
      42,    43,    44,    13,    13,    15,    15,     4,   215,    31,
      32,    14,    14,    35,    36,    14,    38,   224,    40,    13,
      42,    15,   217,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,     3,     4,     5,   204,     7,     8,
       9,    10,    11,    12,    66,    14,    14,    16,    17,    13,
      13,    15,    15,    12,    12,    47,   136,    14,    14,    47,
      58,    14,    31,    32,    18,    13,    35,    36,    12,    38,
       3,    40,    47,    42,   154,    57,    12,    17,    13,    46,
      12,    15,   152,   152,   218,    -1,    55,    56,   154,    58,
      59,    60,    61,    62,    63,    64,    65,    66,     3,     4,
       5,    -1,     7,     8,     9,    10,    11,    12,    -1,    14,
      -1,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      35,    36,    -1,    38,    -1,    40,    -1,    42,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,     3,     4,     5,    -1,     7,     8,     9,    10,
      11,    12,    -1,    14,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    35,    36,    -1,    38,    -1,    40,
      -1,    42,     3,     4,     5,    -1,     7,     8,     9,    10,
      11,    -1,    -1,    14,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    35,    36,    -1,    38,     3,    40,
      -1,    42,    -1,    -1,    -1,    -1,    11,    -1,    -1,    14,
       3,    -1,    -1,    -1,    19,    -1,    -1,    -1,    11,    -1,
      -1,    14,    -1,    16,    -1,    66,    31,    32,    -1,    -1,
      35,    36,    -1,    38,    -1,    40,    -1,    42,    31,    32,
      -1,    -1,    35,    36,    -1,    38,    33,    40,    -1,    42,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
     102,    66,    49,    50,    51,    52,    53,    54,    -1,    -1,
      -1,    -1,    -1,    66,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,   153,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    19,    -1,    49,    50,    51,    52,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    -1,    49,    50,    51,    52,    53,    54,    33,    34,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    51,    52,    53,    54,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    -1,    49,    50,    51,    52,    53,    54,    37,    38,
      -1,    40,    41,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    38,    -1,    40,    41,
      42,    43,    44,    45,    46,    -1,    -1,    49,    50,    51,
      52,    53,    54,    40,    41,    42,    43,    44,    45,    46,
      -1,    -1,    49,    50,    51,    52,    53,    54,    40,    41,
      42,    43,    44,    45,    46,    -1,    -1,    -1,    -1,    51,
      52,    53,    54
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,     7,     8,     9,    10,    68,    69,     4,
       8,     9,    10,     4,     7,     8,     4,     7,     7,     0,
       3,    40,     4,     8,     4,     4,     4,     7,     4,    14,
       4,     4,     6,    69,    70,     3,    13,    15,    69,    16,
       3,    71,     3,    11,    12,    14,    16,    17,    31,    32,
      35,    36,    38,    40,    42,    55,    56,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    69,    72,    76,    77,
      78,    79,    80,    81,    14,    69,    81,    71,     3,    76,
      76,    76,    77,    77,    76,    76,    77,    81,    14,    14,
      72,    14,    12,    12,    79,    14,    47,    14,     3,    73,
      74,    75,    18,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    48,    49,
      50,    51,    52,    53,    54,    12,    13,    81,    15,    15,
      17,    12,    81,    81,    58,    81,    82,    47,    81,    72,
      69,    12,    13,    18,    20,    78,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    79,
      78,    78,    78,    78,    78,    78,    80,    15,    77,    15,
      15,    14,    12,    72,    15,    15,    74,    75,    19,    78,
      16,    80,    83,    19,    47,    72,    72,    81,    82,    72,
      19,    80,    83,    84,    79,    57,    15,    12,    13,    17,
      72,    12,    82,    84,    15,    72
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 412 "simplec.y"
    { return_type = (yyvsp[(1) - (8)]); function_name = (yyvsp[(2) - (8)])->name;
     function_body = flush_decls(new_node2(S_SEQUENCE, IOP_NONE, (yyvsp[(4) - (8)]), flush_decls((yyvsp[(7) - (8)])))); ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 417 "simplec.y"
    { (yyval)=cast_node(ISIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 418 "simplec.y"
    { (yyval)=cast_node(LSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 419 "simplec.y"
    { (yyval)=cast_node(LSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 420 "simplec.y"
    { (yyval)=cast_node(LLSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 421 "simplec.y"
    { (yyval)=cast_node(LLSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 422 "simplec.y"
    { (yyval)=cast_node(SSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 423 "simplec.y"
    { (yyval)=cast_node(SSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 424 "simplec.y"
    { (yyval)=cast_node(SSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 425 "simplec.y"
    { (yyval)=cast_node(SSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 426 "simplec.y"
    { (yyval)=cast_node(CSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 427 "simplec.y"
    { (yyval)=cast_node(CSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 428 "simplec.y"
    { (yyval)=cast_node(ISIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 429 "simplec.y"
    { (yyval)=cast_node(LSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 430 "simplec.y"
    { (yyval)=cast_node(LSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 431 "simplec.y"
    { (yyval)=cast_node(LLSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 432 "simplec.y"
    { (yyval)=cast_node(LLSIZE, DATA_UNSIGNED, NULL); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 433 "simplec.y"
    { (yyval)=cast_node(SSIZE, DATA_SIGNED, NULL); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 434 "simplec.y"
    { (yyval)=cast_node(SSIZE, DATA_SIGNED, NULL); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 435 "simplec.y"
    { (yyval)=cast_node(CSIZE, DATA_SIGNED, NULL); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 436 "simplec.y"
    { (yyval)=cast_node(ISIZE, DATA_SIGNED, NULL); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 437 "simplec.y"
    { (yyval)=cast_node(LSIZE, DATA_SIGNED, NULL); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 438 "simplec.y"
    { (yyval)=cast_node(LSIZE, DATA_SIGNED, NULL); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 439 "simplec.y"
    { (yyval)=cast_node(LLSIZE, DATA_SIGNED, NULL); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 440 "simplec.y"
    { (yyval)=cast_node(LLSIZE, DATA_SIGNED, NULL); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 441 "simplec.y"
    { (yyval)=cast_node(FLOAT_SIZE, DATA_FLOAT, NULL); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 442 "simplec.y"
    { (yyval)=new_node1(E_PTR, IOP_NONE, (yyvsp[(1) - (2)])); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 445 "simplec.y"
    { (yyval)=new_node0(S_NOP, IOP_NONE); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 446 "simplec.y"
    { (yyval)=new_node0(S_NOP, IOP_NONE); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 447 "simplec.y"
    { (yyval)=declare_var((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]), 0); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 448 "simplec.y"
    { (yyval)=sequence_node((yyvsp[(1) - (4)]), declare_var((yyvsp[(3) - (4)]), (yyvsp[(4) - (4)]), 0)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 451 "simplec.y"
    { (yyval)=new_node0(S_NOP, IOP_NONE); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 452 "simplec.y"
    { (yyval)=sequence_node((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 456 "simplec.y"
    { apply_type((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)])); (yyval)=(yyvsp[(2) - (3)]); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 457 "simplec.y"
    { (yyval)=(yyvsp[(1) - (2)]); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 458 "simplec.y"
    { (yyval)=new_node0(S_NOP, IOP_NONE); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 459 "simplec.y"
    { (yyval)=new_node1(S_RETURN, IOP_NONE, (yyvsp[(2) - (3)])); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 460 "simplec.y"
    { (yyval)=new_node1(S_BREAK, IOP_NONE, (yyvsp[(2) - (2)])); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 461 "simplec.y"
    { (yyval)=new_node1(S_CONTINUE, IOP_NONE, (yyvsp[(2) - (2)])); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 462 "simplec.y"
    { (yyval)=new_node1(S_CATCHB, IOP_NONE,
							    new_node2(S_WHILE, IOP_NONE, (yyvsp[(3) - (5)]),
								      new_node1(S_CATCHC, IOP_NONE, (yyvsp[(5) - (5)])))); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 466 "simplec.y"
    { (yyval)=new_node1(S_CATCHB, IOP_NONE,
				   sequence_node(new_node1(S_CATCHC, IOP_NONE, (yyvsp[(2) - (7)])),
						 new_node2(S_WHILE, IOP_NONE, (yyvsp[(5) - (7)]),
							   new_node1(S_CATCHC, IOP_NONE, (yyvsp[(2) - (7)]))))); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 471 "simplec.y"
    { (yyval)=sequence_node((yyvsp[(3) - (9)]),
				       new_node1(S_CATCHB, IOP_NONE,
						 new_node2(S_WHILE, IOP_NONE, (yyvsp[(5) - (9)]),
							   sequence_node(new_node1(S_CATCHC, IOP_NONE, (yyvsp[(9) - (9)])), (yyvsp[(7) - (9)]))))); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 475 "simplec.y"
    { (yyval)=new_node3(S_IFTHEN, IOP_NONE, (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 476 "simplec.y"
    { (yyval)=new_node3(S_IFTHEN, IOP_NONE, (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)]), new_node0(S_NOP, IOP_NONE)); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 477 "simplec.y"
    { (yyval)=flush_decls((yyvsp[(2) - (3)])); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 479 "simplec.y"
    { (yyval)=new_node1(S_CATCHB, IOP_NONE, new_node2(S_SWITCH, IOP_NONE, (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)]))); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 480 "simplec.y"
    { (yyval)=sequence_node(new_node1(S_CASE, IOP_NONE, (yyvsp[(2) - (4)])), (yyvsp[(4) - (4)])); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 481 "simplec.y"
    { (yyval)=sequence_node(new_node0(S_CASE, IOP_NONE), (yyvsp[(3) - (3)])); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 485 "simplec.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 486 "simplec.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 487 "simplec.y"
    { (yyval)=sequence_node((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 488 "simplec.y"
    { (yyval)=sequence_node((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 492 "simplec.y"
    { (yyval) = declare_var(default_type, (yyvsp[(1) - (1)]), 1); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 493 "simplec.y"
    { add_array_dim((yyvsp[(1) - (4)])->children[0], (yyvsp[(3) - (4)])); (yyval) = (yyvsp[(1) - (4)]); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 494 "simplec.y"
    { add_array_dim((yyvsp[(1) - (3)])->children[0], make_ast_num("-1"));  (yyval) = (yyvsp[(1) - (3)]); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 498 "simplec.y"
    { self_check((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));
  	                    (yyval) = sequence_node((yyvsp[(1) - (3)]), new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)])->children[0], (yyvsp[(3) - (3)]))); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 500 "simplec.y"
    { self_check((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));
  	                    (yyval) = sequence_node((yyvsp[(1) - (3)]), new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)])->children[0], (yyvsp[(3) - (3)]))); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 505 "simplec.y"
    { check_ast_var((yyvsp[(1) - (1)])); (yyval)=(yyvsp[(1) - (1)]); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 506 "simplec.y"
    { (yyval)=add_array_ref((yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 510 "simplec.y"
    { (yyval)=(yyvsp[(1) - (1)]); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 511 "simplec.y"
    { (yyval)=(yyvsp[(1) - (1)]); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 512 "simplec.y"
    { (yyval)=(yyvsp[(2) - (3)]); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 513 "simplec.y"
    { (yyval)=cast_node((yyvsp[(2) - (4)])->wsize, (yyvsp[(2) - (4)])->dtype, (yyvsp[(4) - (4)])); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 514 "simplec.y"
    { (yyval)=new_node1(E_UNOP, IOP_ISZERO, (yyvsp[(2) - (2)])); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 515 "simplec.y"
    { (yyval)=new_node1(E_UNOP, IOP_NOT, (yyvsp[(2) - (2)])); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 516 "simplec.y"
    { (yyval)=new_node1(E_UNOP, IOP_NEG, (yyvsp[(2) - (2)])); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 517 "simplec.y"
    { (yyval)=sizeof_node((yyvsp[(3) - (4)])); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 518 "simplec.y"
    { check_ast_var((yyvsp[(1) - (2)])); (yyval)=new_node2(E_PASSIGN, IOP_NONE, (yyvsp[(1) - (2)]), new_node2(E_BINOP, IOP_ADD, (yyvsp[(1) - (2)]), make_ast_num("1"))); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 519 "simplec.y"
    { check_ast_var((yyvsp[(1) - (2)])); (yyval)=new_node2(E_PASSIGN, IOP_NONE, (yyvsp[(1) - (2)]), new_node2(E_BINOP, IOP_SUB, (yyvsp[(1) - (2)]), make_ast_num("1"))); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 520 "simplec.y"
    { check_ast_var((yyvsp[(2) - (2)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(2) - (2)]), new_node2(E_BINOP, IOP_SUB, (yyvsp[(2) - (2)]), make_ast_num("1"))); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 521 "simplec.y"
    { check_ast_var((yyvsp[(2) - (2)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(2) - (2)]), new_node2(E_BINOP, IOP_ADD, (yyvsp[(2) - (2)]), make_ast_num("1"))); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 522 "simplec.y"
    { check_ast_var((yyvsp[(2) - (2)])); (yyval)=new_node1(E_DEREF, IOP_NONE, (yyvsp[(2) - (2)])); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 523 "simplec.y"
    { check_ast_var((yyvsp[(2) - (2)])); (yyval)=new_node1(E_PTR, IOP_NONE, (yyvsp[(2) - (2)])); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 524 "simplec.y"
    { (yyval)=new_node2(E_FUNCALL, IOP_NONE, (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 528 "simplec.y"
    { (yyval)=(yyvsp[(1) - (1)]); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 529 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_MUL, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 530 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_ADD, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 531 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_SUB, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 532 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_DIV, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 533 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_REM, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 534 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_LSHIFT, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 535 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_RSHIFT, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 536 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_LESS, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 537 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_LESSEQUAL, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 538 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_GREATER, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 539 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_GREATEREQUAL, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 540 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_EQUAL, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 541 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_NOTEQUAL, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 542 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_AND, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 543 "simplec.y"
    { (yyval)=new_node2(E_BINOP, IOP_XOR, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 544 "simplec.y"
    { (yyval)=new_node1(E_UNOP, IOP_NOT,
					     new_node2(E_BINOP, IOP_AND,
						       new_node1(E_UNOP, IOP_NOT, (yyvsp[(1) - (3)])),
						       new_node1(E_UNOP, IOP_NOT, (yyvsp[(3) - (3)])))); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 548 "simplec.y"
    { (yyval)=new_node2(E_CAND, IOP_NONE, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 549 "simplec.y"
    { (yyval)=new_node1(E_UNOP, IOP_ISZERO,
					     new_node2(E_CAND, IOP_NONE,
						       new_node1(E_UNOP, IOP_ISZERO, (yyvsp[(1) - (3)])),
						       new_node1(E_UNOP, IOP_ISZERO, (yyvsp[(3) - (3)])))); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 556 "simplec.y"
    { (yyval)=(yyvsp[(1) - (1)]); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 557 "simplec.y"
    { (yyval)=new_node3(E_QUESCOLON, IOP_NONE, (yyvsp[(1) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 561 "simplec.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 562 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 563 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]),
					      new_node2(E_BINOP, IOP_XOR, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]))); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 565 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]),
					      new_node2(E_BINOP, IOP_AND, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]))); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 567 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]),
					      new_node2(E_BINOP, IOP_MUL, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]))); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 569 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]),
					      new_node2(E_BINOP, IOP_ADD, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]))); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 571 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]),
					      new_node2(E_BINOP, IOP_DIV, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]))); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 573 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]),
					      new_node2(E_BINOP, IOP_REM, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]))); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 575 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]),
			                      new_node2(E_BINOP, IOP_SUB, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]))); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 577 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]),
					      new_node1(E_UNOP, IOP_NOT,
							new_node2(E_BINOP, IOP_AND,
								  new_node1(E_UNOP, IOP_NOT, (yyvsp[(1) - (3)])),
								  new_node1(E_UNOP, IOP_NOT, (yyvsp[(3) - (3)]))))); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 582 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]),
					      new_node2(E_BINOP, IOP_LSHIFT, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]))); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 584 "simplec.y"
    { check_ast_var((yyvsp[(1) - (3)])); (yyval)=new_node2(E_ASSIGN, IOP_NONE, (yyvsp[(1) - (3)]),
					      new_node2(E_BINOP, IOP_RSHIFT, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]))); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 590 "simplec.y"
    { (yyval)=(yyvsp[(1) - (1)]); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 591 "simplec.y"
    { (yyval)=new_node2(E_SEQUENCE, IOP_NONE, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 596 "simplec.y"
    { (yyval) = make_ast_num("1"); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 597 "simplec.y"
    { (yyval)=(yyvsp[(1) - (1)]); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 602 "simplec.y"
    { (yyval)=(yyvsp[(1) - (1)]); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 603 "simplec.y"
    { (yyval)=(yyvsp[(2) - (3)]); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 607 "simplec.y"
    { (yyval)=new_node2(E_SEQUENCE, IOP_NONE, (yyvsp[(1) - (1)]), NULL); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 608 "simplec.y"
    { (yyval)=new_node2(E_SEQUENCE, IOP_NONE, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2887 "simplec.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



