#!/usr/bin/env python

__author__ = "Jonah Brooks"

'''translate the AST parsed by compiler.parse() and generate C code.
   based on Siek's code, but added assignments and for statement.'''

'''Implements the P_1 cfg defined as follows:
   program : module
   module : stmt+
   stmt : (simple_stmt | for_stmt) NEWLINE
   simple_stmt : "print" expr ("," expr)*
               | name "=" expr

   for_stmt : "for" name "in" "range" "(" expr ")" ":" simple_stmt

   expr : name
        | decint
        | "-" expr
        | expr "+" expr
        | "(" expr ")"
'''

'''Implements the P-2 cfg defined as follows:
   program : module
   module : stmt+
   stmt : (simple_stmt | if_stmt | for_stmt) NEWLINE

   simple_stmt : "print" expr ("," expr)*
               | int_name "=" int_expr
               | bool_name "=" bool_expr

   expr : int_expr | bool_expr

   if_stmt : "if" bool_expr ":" (simple_stmts | suite)

   for_stmt : "for" name "in" "range" "(" int_expr ")" ":" (simple_stmts | suite)

   simple_stmts : simple_stmt (";" simple_stmt)+
   suite : NEWLINE INDENT stmt+ DEDENT

   int_expr : int_name
            | decint
            | "-" int_expr
            | int_expr "+" int_expr
            | "(" int_expr ")"
            | int_expr "if" bool_expr "else" int_expr

   bool_expr : bool_name
             | bool_expr "and" bool_expr
             | bool_expr "or" bool_expr
             | "not" bool_expr
             | "(" bool_expr ")"
             | int_expr (comp_op int_expr)+
             | "True"
             | "False"
             | "(" bool_expr "if" bool_expr "else" bool_expr ")"   [UPDATE]

   comp_op : '<' | '>' | '==' | '>=' | '<=' | '<>' | '!='
'''

import sys
logs = sys.stderr
import ast # ast.parse, ast.dump  
from ast import *                


prelude = set([])
first_pass = True

def generate_c(n):
    global prelude
    if isinstance(n, AST):
      if isinstance(n, Module):
          out_prelude = ''
          if first_pass == False:
            for assignment in prelude:
              out_prelude = out_prelude + "int " + assignment + " = 0;\n"
            return "\n".join(["#include <stdio.h>",
                               "int main()",
                               "{", 
                               out_prelude,
                               '\n'.join([generate_c(node) for node in n.body]), 
                               "return 0;",
                               "}"])
          else:
            prelude = set([])
            [generate_c(node) for node in n.body]
            return prelude
      elif isinstance(n, Print): 
          out = 'printf(\"'
          for i in range(len(n.values)):
            out = out + '%d'
            if i < len(n.values)-1:
              out = out + ' '
          out = out + '\\n\"'
          for node in n.values:
            out = out + ', %s' % generate_c(node)
          out = out + ');'
  
          return out
      elif isinstance(n, Num):
          return '%d' % n.n
      elif isinstance(n, Name):
          return '%s' % n.id
      elif isinstance(n, UnaryOp) and isinstance(n.op, USub):
          return '(-%s)' % generate_c(n.operand)
      elif isinstance(n, BinOp) and isinstance(n.op, Add):
          out = ''
          out = out + '%s + ' % generate_c(n.left)
          out = out + '%s' % generate_c(n.right)
          return out
      elif isinstance(n, Assign):
          name = generate_c(n.targets[0])
          out = ''
          if name in prelude:
              out = '%s = ' % name
              out = out + generate_c(n.value) + ';'
          else:
              prelude.add(name) # Just add this to prelude; nothing else matters since this is a first pass
          return out
      elif isinstance(n, For) and isinstance(n.iter, Call) and n.iter.func.id == "range":
          # for_stmt : "for" name "in" "range" "(" expr ")" ":" simple_stmt
          out = ''
          index_var = generate_c(n.target)
          orig_index_var = index_var
          if index_var in prelude:
              while index_var in prelude:
                index_var = index_var + '_' # Add underscores until this name is unique
              out = out + 'int %s = 0;\n' % index_var
              if first_pass == False:
                prelude.add(index_var) # Won't declare at top, will prevent duplicates later
          else:
              prelude.add(index_var) # Just add this to prelude; nothing else matters since this is a first pass
          reference_var = generate_c(n.iter.args[0]) 
          orig_reference_var = reference_var
          reference_var = 'tmp'
          while reference_var in prelude:
            reference_var = reference_var + '_'
          if first_pass == False:
            prelude.add(reference_var) # Won't declare at top, will prevent duplicates later
          out = out + 'int ' + reference_var + ' = ' + orig_reference_var + ';\n'
          out = out + 'for(' + index_var + ' = 0; '
          out = out + index_var + ' < ' + reference_var + '; '
          out = out + index_var + '++' + ')\n'
          out = out + '{\n'
          out = out + '\t' + orig_index_var + ' = ' + index_var + ';\n'
          out = out + '\t' + '\n\t'.join([generate_c(stm) for stm in n.body])
          out = out + '\n}'
          return out
      else:
          raise sys.exit('Error in generate_c: unrecognized AST node: %s' % n)
    else:
        raise sys.exit('Error in generate_c: unrecognized non-AST of type %s: %s' % (str(type(n)), str(n)))


if __name__ == "__main__":        
    try:
        astparse = ast.parse("\n".join(sys.stdin.readlines()))
        print >> logs, ast.dump(astparse) # debug
        generate_c(astparse) # Fill prelude with variable assignments
        first_pass = False
        print generate_c(astparse)
    except Exception, e:
        print >> logs, e.args[0]
        exit(-1)
