# $Id: MyDD.py,v 1.1 2001/11/05 19:53:33 zeller Exp $
# Template for adapting delta debugging.  Areas to customize are
# tagged with `FIXME'.

import DD
import commands
import string

class MyDD(DD.DD):
    def __init__(self):
        DD.DD.__init__(self)
        
    def _test(self, deltas):
	# FIXME: Set up a test function that takes a set of deltas and
        # returns either self.PASS, self.FAIL, or self.UNRESOLVED.
        if (deltas == []):
            return self.PASS

        initFound = False
        
        commands.getstatusoutput("rm -rf bugtemp.c")
        commands.getstatusoutput("rm -rf bugp.exe bugp")
        f = open("bugtemp.c","w")
        print ("NOW DELTAS ARE: ") + str(deltas)
        for (i, d) in deltas:
            if (d.find("initializeGame") != -1):
                initFound = True
            f.write(d)
        f.close()
        if not initFound:
            return self.PASS
        (status, output) = commands.getstatusoutput("gcc -o bugp bugtemp.c dominion.o rngs.o -fprofile-arcs -ftest-coverage; ./bugp")
        print "OUTPUT OF GCC:"
        print output
        
        if (output.find("Segmentation") != -1):
            print "FAILED"
            return self.FAIL
        if (output.find("OK:") != -1):
            print "PASSED"
            return self.PASS
        return self.UNRESOLVED

if __name__ == '__main__':
    deltas = []
    n = 0
    for l in open("bug2.c"):
        deltas.append((n,l))
        n = n + 1
    print "THE DELTAS IS: " + str(deltas)
    # FIXME: Insert your deltas here

    mydd = MyDD()
    
    print "Simplifying failure-inducing input..."
    c = mydd.ddmin(deltas)              # Invoke DDMIN
    print "The 1-minimal failure-inducing input is", c
    print "Removing any element will make the failure go away."
    print
    
    #print "Isolating the failure-inducing difference..."
    #(c, c1, c2) = mydd.dd(deltas)	# Invoke DD
    #print "The 1-minimal failure-inducing difference is", c
    #print c1, "passes,", c2, "fails"
    commands.getstatusoutput("rm -rf minimal.c")
    m = open("minimal.c","w")
    for (i,l) in c:
        m.write(l)
    m.close()
        



# Local Variables:
# mode: python
# End:
