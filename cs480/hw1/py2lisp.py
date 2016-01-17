#!/usr/bin/env python

__author__ = "Liang Huang"

''' Convert the following subset of Python into Lisp

   program : module
   module : stmt
   stmt : simple_stmt NEWLINE
   simple_stmt : "print" expr

   expr : decint
        | "-" expr
        | expr "+" expr
        | expr "*" expr
        | "(" expr ")"

'''

import sys
logs = sys.stderr
import compiler
from compiler.ast import *

def generate_c(n):
    if isinstance(n, Module):
        return generate_c(n.node) + "\n"
    elif isinstance(n, Stmt):  # only one statement; TODO: handle multiple statements
        return generate_c(n.nodes[0])
    elif isinstance(n, Printnl): # only support single item; TODO: handle multiple items
        return generate_c(n.nodes[0])
    elif isinstance(n, Const):
        return '%d' % n.value
    elif isinstance(n, UnarySub):
        return '(- %s)' % generate_c(n.expr)
    elif isinstance(n, Add):
        return '(+ %s %s)' % (generate_c(n.left), generate_c(n.right))
    elif isinstance(n, Mul):
        return '(* %s %s)' % (generate_c(n.left), generate_c(n.right))
    else:
        raise sys.exit('Error in generate_c: unrecognized AST node: %s' % n)

if __name__ == "__main__":        
    try:
        ast = compiler.parse("\n".join(sys.stdin.readlines()))
        print >> logs, ast # debug
        print generate_c(ast)
    except Exception, e:
        print >> logs, e.args[0]
        exit(-1)
