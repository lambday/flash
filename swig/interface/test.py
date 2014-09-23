#/usr/bin/env python

import sys
from modcomputation import ComputeFoo, ComputeCube

def test(n):
	c = ComputeFoo()
	for i in range(n):
		c.compute(i)
	c = ComputeCube()
	for i in range(n):
		c.compute(i)

if __name__ == "__main__":
	if len(sys.argv) > 1:
		test(int(sys.argv[1]))
	else:
		raise Exception
