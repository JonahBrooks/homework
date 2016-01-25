#!/usr/bin/env python

__author__ = "Jonah Brooks"

'''translate the AST parsed by compiler.parse() and generate C code.
   based on Siek's code, but added assignments and for statement.'''

'''Implements the P-2 cfg defined as follows:
   program : module
   module : stmt+
**   stmt : (simple_stmt | if_stmt | for_stmt) NEWLINE

**   simple_stmt : "print" expr ("," expr)*
               | int_name "=" int_expr
               | bool_name "=" bool_expr

**   expr : int_expr | bool_expr

**   if_stmt : "if" bool_expr ":" (simple_stmts | suite)

**   for_stmt : "for" name "in" "range" "(" int_expr ")" ":" (simple_stmts | suite)

**   simple_stmts : simple_stmt (";" simple_stmt)+
**   suite : NEWLINE INDENT stmt+ DEDENT

**   int_expr : int_name
            | decint
            | "-" int_expr
            | int_expr "+" int_expr
            | "(" int_expr ")"
            | int_expr "if" bool_expr "else" int_expr

**   bool_expr : bool_name
             | bool_expr "and" bool_expr
             | bool_expr "or" bool_expr
             | "not" bool_expr
             | "(" bool_expr ")"
             | int_expr (comp_op int_expr)+
             | "True"
             | "False"
             | "(" bool_expr "if" bool_expr "else" bool_expr ")"   [UPDATE]

[]   comp_op : '<' | '>' | '==' | '>=' | '<=' | '<>' | '!='
'''

import sys
logs = sys.stderr
import ast # ast.parse, ast.dump  
from ast import *                


prelude = {'True': 'bool', 'False': 'bool'} # Set of format {'variable' = 'type', etc}
first_pass = True

def generate_c(n):
    global prelude

    def isboolexpr(n): 
      global prelude
      ''' Implements: 
           bool_expr : bool_name
                     | bool_expr "and" bool_expr
                     | bool_expr "or" bool_expr
                     | "not" bool_expr
                     | "(" bool_expr ")"
                     | int_expr (comp_op int_expr)+
                     | "True"
                     | "False"
                     | "(" bool_expr "if" bool_expr "else" bool_expr ")"   [UPDATE]
      '''
      if isinstance(n,BoolOp) or isinstance(n,Compare) \
          or (isinstance(n,UnaryOp) and isinstance(n.op,Not) )\
          or (isinstance(n,Name) and n.id in prelude and prelude[n.id] == 'bool')\
          or (isinstance(n,Name) and n.id == 'True' and not n.id in prelude) \
          or (isinstance(n,Name) and n.id == 'False' and not n.id in prelude) \
          or (isinstance(n,IfExp) and isboolexpr(n.body)):
        return True
      return False

    if isinstance(n, AST):
      if isinstance(n, Module):
          out_prelude = ''
          if first_pass == False:
            for assignment in prelude:
              if assignment == "True":
                out_prelude = out_prelude + "bool True = true;\n"
              else:
                out_prelude = out_prelude + prelude[assignment] + " " + assignment + " = " + ('0' if prelude[assignment] == 'int' else 'false') + ";\n"
            return "\n".join(["#include <stdio.h>",
                              "#include <stdbool.h>",
                               "int main()",
                               "{", 
                               out_prelude,
                               '\n'.join([generate_c(node) for node in n.body]), 
                               "return 0;",
                               "}"])
          else:
            prelude = {'True': 'bool', 'False': 'bool'}
            [generate_c(node) for node in n.body]
            return prelude
      elif isinstance(n, Print): 
          out = 'printf(\"'
          for i,v in zip(range(len(n.values)),n.values):
            if isboolexpr(v):
              out = out + "%s"
            else:
              out = out + "%d"
            if i < len(n.values)-1:
              out = out + ' '
          out = out + '\\n\"'
          for node in n.values:
            if isboolexpr(node):
              out = out + ', ((%s)?"True":"False")' % generate_c(node)
            else:
              out = out + ', %s' % generate_c(node)
          out = out + ');'
  
          return out
      elif isinstance(n, Num):
          return '%d' % n.n
      elif isinstance(n, Name):
          name = n.id
          out = '%s' % n.id
          if name in prelude:
            out = '%s' % n.id 
          elif name == "True":
            out = "true"
          elif name == "False":
            out = "false"
          return out
      elif isinstance(n, UnaryOp) and isinstance(n.op, USub):
          return '(-(%s))' % generate_c(n.operand)
      elif isinstance(n, UnaryOp) and isinstance(n.op, Not):
          return '(!(%s))' % generate_c(n.operand)
      elif isinstance(n, BinOp) and isinstance(n.op, Add):
          out = ''
          out = out + '%s + ' % generate_c(n.left)
          out = out + '%s' % generate_c(n.right)
          return out
      elif isinstance(n, BoolOp) and isinstance(n.op, And):
          out = ''
          out = out + '((%s) && ' % generate_c(n.values[0])
          out = out + '(%s))' % generate_c(n.values[1])
          return out
      elif isinstance(n, BoolOp) and isinstance(n.op, Or):
          out = ''
          out = out + '((%s) || ' % generate_c(n.values[0])
          out = out + '(%s))' % generate_c(n.values[1])
          return out
      elif isinstance(n, Compare):
          # comp_op : '<' | '>' | '==' | '>=' | '<=' | '<>' | '!='
          compops = {"<class '_ast.Lt'>": ' < ', 
                     "<class '_ast.Gt'>": ' > ', 
                     "<class '_ast.Eq'>": ' == ', 
                     "<class '_ast.GtE'>": ' >= ', 
                     "<class '_ast.LtE'>": ' <= ', 
                     "<class '_ast.NotEq'>": ' <> ', 
                     "<class '_ast.NotEq'>": ' != '}
          out = '('
          out = out + generate_c(n.left) + compops[str(type(n.ops[0]))] + generate_c(n.comparators[0])
          for op, comp, i in zip(n.ops, n.comparators, range(len(n.comparators))):
            if i > 0:
              out = out + "&& (" + generate_c(n.comparators[i-1]) + compops[str(type(op))] + generate_c(comp) + ")"
          out = out + ")"
          return out;
      elif isinstance(n, Assign):
          name = n.targets[0].id
          out = ''
          vartype = 'int'
          if isboolexpr(n.value):
             vartype = 'bool'
          if name in prelude:
              # Check if generate_c(n.value) matches type with prelude[name]
              if prelude[name] != vartype:
                raise sys.exit('Variable type mismatch for %s variable %s being assigned: %s value' % (str(prelude[name]), str(name), str(vartype)))
              out = '%s = ' % name
              out = out + generate_c(n.value) + ';'
          else:
              prelude[name] = vartype # Just add this to prelude; nothing else matters since this is a first pass
          return out
      elif isinstance(n, For) and isinstance(n.iter, Call) and n.iter.func.id == "range":
          # for_stmt : "for" name "in" "range" "(" int_expr ")" ":" (simple_stmts | suite)
          out = ''
          index_var = generate_c(n.target)
          orig_index_var = index_var
          if index_var in prelude:
              while index_var in prelude:
                index_var = index_var + '_' # Add underscores until this name is unique
              out = out + 'int %s = 0;\n' % index_var
              if first_pass == False:
                prelude[index_var] = 'int' # Won't declare at top, will prevent duplicates later
          else:
              prelude[index_var] = 'int' # Just add this to prelude; nothing else matters since this is a first pass
          reference_var = generate_c(n.iter.args[0]) 
          orig_reference_var = reference_var
          reference_var = 'tmp'
          while reference_var in prelude:
            reference_var = reference_var + '_'
          if first_pass == False:
            prelude[reference_var] = 'int' # Won't declare at top, will prevent duplicates later
          out = out + 'int ' + reference_var + ' = ' + orig_reference_var + ';\n'
          out = out + 'for(' + index_var + ' = 0; '
          out = out + index_var + ' < ' + reference_var + '; '
          out = out + index_var + '++' + ')\n'
          out = out + '{\n'
          out = out + '\t' + orig_index_var + ' = ' + index_var + ';\n'
          out = out + '\t' + '\n\t'.join([generate_c(stm) for stm in n.body])
          out = out + '\n}'
          return out
      elif isinstance(n, If):
          #  if_stmt : "if" bool_expr ":" (simple_stmts | suite)
          out = 'if (' + generate_c(n.test) + ")\n{\n"
          for stm in n.body:
            out = out + "\t" + generate_c(stm) + "\n"
          out = out + "}"
          return out
      elif isinstance(n, IfExp):
          # "(" bool_expr "if" bool_expr "else" bool_expr ")"
          out = "( " + generate_c(n.test) + " ? " + generate_c(n.body) + " : " + generate_c(n.orelse) + " )"
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
