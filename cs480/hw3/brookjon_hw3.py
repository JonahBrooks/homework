#!/usr/bin/env python2.7

__author__ = 'Jonah Brooks'

''' Made using the hw3_p1.5james.py provided in class. 
''' 

import sys, traceback
logs = sys.stderr

''' P_3 grammar in pure CFG:
   program : module
   module : stmt_list
   stmt : simple_stmt NEWLINE
        | if_stmt NEWLINE 
        | for_stmt NEWLINE
   stmt_list : stmt
             | stmt stmt_listt

   simple_stmt : "print" expr_list
               | name "=" expr

   expr_list : expr
             | expr_list "," expr

   if_stmt : "if" expr ":" simple_stmts 
           | "if" expr ":" NEWLINE INDENT stmt_list DEDENT

   for_stmt : "for" name "in" "range" "(" expr ")" ":" simple_stmts 
            | "for" name "in" "range" "(" expr ")" ":" NEWLINE INDENT stmt_list DEDENT

   simple_stmts : simple_stmt ";" simple_stmt       // Is this right? Or should P_2 have had * here?
                | simple_stmts ";" simple_stmt

   comp_list: expr comp_op expr
            | comp_list comp_op expr

   expr     : name
            | decint
            | "-" expr
            | expr "+" expr
            | "(" expr ")"
            | expr "if" expr "else" expr
            | expr "and" expr
            | expr "or" expr
            | "not" expr
            | comp_list
            | "True"
            | "False"
            | "(" expr "if" expr "else" expr ")"

   comp_op : '<' | '>' | '==' | '>=' | '<=' | '<>' | '!='
'''

###############################################################################
# Lexer

import ply.lex as lex

def my(self, t=None, v=None, lineno=1):
    self.type, self.value = t, v # return None
    
lex.LexToken.__str__ = lambda self: "(%s, %s)" % (self.type, self.value)
lex.LexToken.__init__ = my


class MyLexer(object):

    keywords = {
        'print' : 'PRINT',
        'if' : 'IF',
        'for': 'FOR',
        'else': 'ELSE',
        'range': 'RANGE',
        'in': 'IN',
        'and': 'AND',
        'or': 'OR',
        'not': 'NOT',
        'True': 'TRUE',
        'False': 'FALSE',
        }
    
    ops = { 
        '=': 'ASSIGN',
        '-': 'SUB',
        '+': 'PLUS',
        }

    comp_ops = {
        '<' : 'LT',
        '>' : 'GT',
        '==': 'EQ',
        '>=': 'GE',
        '<=': 'LE',
        '<>': 'NEQ1',
        '!=': 'NEQ2',
        }

    tokens = ['INDENT', 'DEDENT',  
              'COLON',
              'LPAREN',
              'RPAREN',
              'SCOLON',
              'COMMA',
              'INT', 
              'NAME',
              'NEWLINE',
              ] + ops.values() + comp_ops.values() + keywords.values()
    
    t_COMMA = r','

    def t_SUB(self,t): 
      r'-'
      t.fun = Sub
      return t
    def t_PLUS(self,t): 
      r'\+'
      t.fun = Add
      return t

    def t_AND(self,t): 
      r'and'
      t.fun = And
      return t
    def t_OR(self,t): 
      r'or'
      t.fun = Or
      return t
    def t_NOT(self,t): 
      r'not'
      t.fun = Not
      return t
  

    def t_LT(self, t):
      r'\<'
      t.fun = Lt
      t.type = MyLexer.comp_ops.get(t.value, 'LT')  # Check for key words first
      return t

    def t_GT(self,t): 
      r'\>'
      t.fun = Gt
      return t
    def t_EQ(self,t): 
      r'=='
      t.fun = Eq
      return t
    def t_LE(self,t): 
      r'\<='
      t.fun = LtEq
      return t
    def t_GE(self,t): 
      r'\>='
      t.fun = GtEq
      return t
    def t_NEQ1(self, t):
      r'\>\<'
      t.fun = NotEq
      return t
    def t_NEQ2(self, t):
      r'\!='
      t.fun = NotEq
      return t

    t_COLON = r':'
    t_LPAREN = r'\('
    t_RPAREN = r'\)'
    t_SCOLON = r';'

    def get_newline(self):
        return lex.LexToken('NEWLINE', 1, self.lexer.lineno)

    def get_dedent(self):
        return lex.LexToken('DEDENT', 1, self.lexer.lineno) # important
   
    def t_indent(self, t):
        r'\n[ ]*' # can only use space in indentations (first and final \n added)
        t.lexer.lineno += 1 # keep track of linenos
        if t.lexer.lexpos < len(t.lexer.lexdata) \
                and t.lexer.lexdata[t.lexer.lexpos] == "\n": # empty but not last line
            return None # ignore pure empty lines
        width = len(t.value) - 1 # exclude \n
        last_pos = t.lexer.lexpos - width
        if width == self.indents[-1]:
            return self.get_newline() # same indentation level
        elif width > self.indents[-1]: # one more level
            t.type = 'INDENT'
            t.value = 1
            self.indents.append(width)
            return t
        # try one or more DEDENTS
        ded = 0
        while len(self.indents) > 1:
            self.indents.pop()
            ded += 1
            if width == self.indents[-1]:
                t.type = 'DEDENT'
                t.value = ded # remember how many dedents
                return t
        raise Exception("indent error at line %d:\" %s\"" % (t.lexer.lineno, 
                                                             self.lines[t.lexer.lineno]))

    def t_space(self, t):
        r'[ ]+'
        return None # ignore white space

    #t_ignore = ' \t'                 # ignore spaces

    def t_INT(self, t): # t is the current token
        r'(-?)\d+'
        try:
            t.value = int(t.value)
        except ValueError:
            t.value = 0
            raise Exception("integer value too large", t.value)
        return t

    def t_NAME(self, t):
        r'[a-zA-Z_][a-zA-Z_0-9]*'
        t.type = MyLexer.keywords.get(t.value, 'NAME')  # Check for key words first
        return t
    
    def t_ASSIGN(self, t):
        r'='   # TODO: Make sure this and EQ play nice
        return t

    def t_error(self, t):
        raise Exception("Illegal character '%s' in line %d:\" %s\"" % (t.value[0], 
                                                                       t.lexer.lineno, 
                                                                       self.lines[t.lexer.lineno]))
        t.lexer.skip(1)

    def __init__(self, **kwargs):
        self.lexer = lex.lex(module=self, **kwargs) # build regexps
        self.dedent_balance = 0
        self.indents = [0]

    def input(self, stream):
        # the initial \n is to simplify indent
        # the final \n is to simplify dedent
        stream = "\n" + stream + "\n" 
        self.lexer.lineno -= 1 # remove first line
        self.lexer.input(stream)
        self.lines = stream.split("\n") # internal
        print >> logs, "now lexing..."
        _tokens = [] # can't re-use tokens (needed by lex)
        while True:
            tok = self.lexer.token() # lexer.token
            if not tok:
                break
            if tok.type == 'DEDENT': # multiple dedents
                for _ in range(tok.value):
                    _tokens.append(self.get_newline()) # new instance every time
                    _tokens.append(self.get_dedent())  # new instance every time
                _tokens.append(self.get_newline()) # N.B.: newline after block
            elif tok.type == 'INDENT':
                _tokens.append(self.get_newline()) # new instance every time
                _tokens.append(tok)
            else:
                # normal token
                _tokens.append(tok)
        for tok in _tokens:
            print >> logs, tok, # debug
        print >> logs
        self._tokens = _tokens
        self.tokid = 0 # prepare for token()

    def token(self):
        try:
            self.tokid += 1
            print >> logs, self._tokens[self.tokid]
            return self._tokens[self.tokid]
        except:
            return None

###############################################################################
# Parser

from ast import * # AST node types and dump() function

precedence = (
    ('nonassoc', 'NAME'),
    ('nonassoc', 'FOR'),
    ('nonassoc', 'PRINT'),
    ('left', 'OR'),
    ('left', 'AND'),
    ('right', 'EQ', 'GT', 'LT', 'GE', 'LE', 'NEQ1', 'NEQ2'),
    ('left', 'PLUS'),
    ('right', 'ELSE', 'IF'),
    ('nonassoc', 'ASSIGN', 'RANGE'),
    ('nonassoc', 'SUB', 'NOT')
    )

def p_module(t):
    '''module : stmt_list'''
    t[0] = Module(body=t[1]) # t[1] is already a list

def p_stmt(t): 
    '''stmt : simple_stmt NEWLINE
            | if_stmt NEWLINE 
            | for_stmt NEWLINE '''
    t[0] = t[1]

def p_stmt_list(t): # flattens into a list
    '''stmt_list : stmt 
                 | stmt stmt_list '''
    if len(t) == 2:
        t[0] = [t[1]]
    else:
        t[0] = [t[1]] + t[2]

def p_simple_stmt(t):
    '''simple_stmt : PRINT expr_list
                   | NAME ASSIGN expr '''
    if len(t) == 3:
      t[0] = Print(values=[t[2]]) # t[2] is a list
    else:
      t[0] = Assign(targets=[Name(id=t[1], ctx=Store())], value=t[3])

def p_expr_list(t): 
    '''expr_list : expr
                 | expr_list COMMA expr '''
    if len(t) == 2:
      t[0] = t[1]
    else:
      if isinstance(t[1], list):
        if isinstance(t[3], list):
          t[0] = t[1] + t[3] # In case 2, make a list
        else:
          t[0] = t[1] + [t[3]] # In case 2, make a list
      else:
        t[0] = [t[1], t[3]] # In case 2, make a list

def p_if_stmt(t): 
    '''if_stmt : IF expr COLON simple_stmts
               | IF expr COLON NEWLINE INDENT stmt_list DEDENT '''
    if len(t) == 6:
      t[0] = If(test=t[2], body=t[6]) # Case 2
    else:
      t[0] = If(test=t[2], body=t[4]) # Case 1

def p_for_stmt(t): 
    '''for_stmt : FOR NAME IN RANGE LPAREN expr RPAREN COLON simple_stmts 
                | FOR NAME IN RANGE LPAREN expr RPAREN COLON NEWLINE INDENT stmt_list DEDENT '''
    if len(t) == 10:
      t[0] = For(target=Name(id=t[2], ctx=Store()), 
                 iter=Call(func=Name(id='range', ctx=Load()),
                           args=[t[6]],
                           keywords=[], starargs=None, kwargs=None),
                 body=[t[9]],
                 orelse=[])
    else:
      t[0] = For(target=Name(id=t[2], ctx=Store()), 
                 iter=Call(func=Name(id='range', ctx=Load()),
                           args=[t[6]],
                           keywords=[], starargs=None, kwargs=None),
                 body=t[11], 
                 orelse=[])

def p_simple_statements(t):
    '''simple_stmts : simple_stmt
                    | simple_stmts SCOLON simple_stmt '''
    if len(t) == 2:
      t[0] = t[1]
    else:
      t[0] = t[1] + [t[3]]

def p_expr(t):
    ''' expr : NAME
             | INT
             | SUB expr
             | expr PLUS expr
             | LPAREN expr RPAREN
             | expr IF expr ELSE expr
             | expr AND expr
             | expr OR expr
             | NOT expr
             | comp_list
             | TRUE
             | FALSE
             | LPAREN expr IF expr ELSE expr RPAREN '''

    if len(t) == 2:  # NAME, INT, comp_list, TRUE, FALSE
      if isinstance(t[1], int):
        t[0] = Num(t[1])
      elif isinstance(t[1], str):
        t[0] = Name(id=t[1], ctx=Load())
    elif len(t) == 3:  # SUB, NOT
      if t[1] == '-':
        t[0] = UnaryOp(op=USub(), operand=t[2])
      elif t[1] == 'not':
        t[0] = UnaryOp(op=Not(), operand=t[2])
    elif len(t) == 4:  # PLUS, AND, OR, PAREN
      if t[2] == '+':
        t[0] = BinOp(left=t[1], op=Add(), right=t[3]) # PLUS, AND, OR
      elif t[2] == 'and':
        t[0] = BinOp(left=t[1], op=And(), right=t[3]) # PLUS, AND, OR
      elif t[2] == 'or':
        t[0] = BinOp(left=t[1], op=Or(), right=t[3]) # PLUS, AND, OR
      else: #Paren
        if isinstance(t[2], int):
          t[0] = Num(t[2])
        elif isinstance(t[2], str):
          t[0] = Name(id=t[2], ctx=Load())
        else:
          t[0] = id=t[2]
    else: # IF, ELSE
      t[0] = IfExp(test=t[3], body=t[1], orelse=t[5])

def p_comp_list(t):
    ''' comp_list : expr comp_op expr
                  | comp_list comp_op expr '''
    if not isinstance(t[1], list):
      t[0] = Compare(left=t[1], ops=[t[2]], comparators=[t[3]])
    else:
      t[0] = Compare(left=t[1][1], ops=[t[2]] + [t[1][2]], comparators=[t[1][3]] + [t[3]])
      # TODO: This is broken, and I'm not sure why

def p_comp_op(t):
    '''comp_op : LT 
               | GT 
               | EQ 
               | GE 
               | LE 
               | NEQ1 
               | NEQ2 '''
    if t[1] == "<":
      t[0] = Lt
    # TODO: Fill in the rest of these if they are needed when comp_list works

def p_error(t):
    raise Exception("Syntax error (on line %d): '%s'" % (t.lineno, t.value))


import ply.yacc as yacc

def mydump(r):
    if isinstance(r, AST):
        return dump(r)
    elif isinstance(r, list):
        return "[%s]" % (", ".join(map(dump, r)))
    return str(r)

# Format the result message that the parser produces when running in debug mode.
def myresult(r):
    repr_str = mydump(r) # N.B. lhuang hack: dump(r) instead of repr(r)
    if '\n' in repr_str:
        repr_str = repr(repr_str)
    if len(repr_str) > 85:
        # truncate long lines; N.B. lhuang hack: xxxxx ... xxx
        repr_str = "%s...%s" % (repr_str[:70], repr_str[-15:])
    result = repr_str #(type(r).__name__, id(r), repr_str)
    return result

# Format stack entries when the parser is running in debug mode
def mystack(r):
    repr_str = mydump(r)
    if '\n' in repr_str:
        repr_str = repr(repr_str)
    if len(repr_str) < 30:
        return repr_str
    else:
        # lhuang hack
        return "%s...%s" % (repr_str[:15], repr_str[-15:]) #'<%s @ 0x%x>' % (type(r).__name__, id(r))


yacc.format_result = myresult
yacc.format_stack_entry = mystack

tokens = MyLexer.tokens # important

yacc.yacc()

###############################################################################
# Main

if __name__ == "__main__":

    try:
        mylexer = MyLexer()
        tree = yacc.parse(sys.stdin.read(), lexer=mylexer, debug=1)
        print >> logs, dump(tree)
        from hw2_liang import generate_c 
        print generate_c(tree)

    except Exception as e:
        print >> logs, e
        traceback.print_exc(file=logs)
