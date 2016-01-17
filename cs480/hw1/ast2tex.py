#!/usr/bin/env python

__author__ = 'Liang Huang'

'''convert Python AST to tikz forest trees'''

import sys
logs = sys.stderr

import compiler
from compiler.ast import *

def dump(node, latex=False):
    indent = "  " if latex else "| "
    
    def _dump(node, level=0, attrname=""):
        indents = indent * level 
        subindent = indent * (level+1)

        realname = "" if level == 0 else (attrname if latex else ".%s: " % attrname)
        if not latex:
            prefix = "%s%s" % (indents, realname)
            suffix = ""
        else:
            prefix = "%s[ " % indents
            suffix = "%s%s" % (", edge label={node[midway,fill=white!10] {%s} }" % realname,
                               "]" if type(node) in [int, str, list] else "")
                  
        if isinstance(node, Node):
            print "%s%s%s" % (prefix, node.__class__.__name__, suffix)
            for name, attr in reversed(list(node.__dict__.iteritems())):
                if name in ['dest', 'lineno', 'doc', 'flags'] or attr is None:
                    continue
                if isinstance(attr, list):
                    if attr != []:
                        for i, item in enumerate(attr):
                            if item is not None:
                                _dump(item, level+1, "%s[%d]" % (name, i)) # nodes[0]
                    else:
                        _dump(attr, level+1, name)  # args: []
                else:
                    _dump(attr, level+1, name) # normal node
            if latex:
                print "%s]" % indents
        elif isinstance(node, int):
            print "%s%s%s" % (prefix, node, suffix)
        elif node == []:
            print "%s%s%s" % (prefix, "{[\,]}" if latex else "[]", suffix)
        elif isinstance(node, str):
            print "%s\"%s\"%s" % (prefix, node, suffix)
        else:
            raise NotImplementedError(str(node))
        
    _dump(node)

if __name__ == "__main__":        
    try:
        program = sys.stdin.readlines()
        ast = compiler.parse("\n".join(program))
        print >> logs, ast # debug

        dump(ast)

        print >> logs, "LaTeX source saved to output.tex; please run 'pdflatex output'."

        sys.stdout = open("output.tex", "wt")
        print
        print "\\documentclass{article}"
        print "\\usepackage{forest}"
        print "\\renewcommand{\\familydefault}{\\ttdefault}"
        print "\\begin{document}"
        if len(program) < 11:
            print "\\hspace{-4cm}\\resizebox{19cm}{!}{" 
            suffix = "}"
        else:
            print "\\hspace{-4cm}\\resizebox{!}{19cm}{\\rotatebox{90}{" 
            suffix = "} }"
        
        print "\\begin{forest}"
        print "for tree={ellipse,draw, l sep=20pt, s sep=20pt, edge=->}"
        dump(ast, latex=True)
        print "\\end{forest}"
        print suffix                 # close braces
        print "\\end{document}"
    except Exception, e:
        print >> logs, "bad:" + e.args[0]
        exit(-1)
