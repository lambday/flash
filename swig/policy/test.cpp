#include <cstdlib>
#include <Computation.h>

void test(int n)
{
	ComputeFoo f;
	for (int i = 0; i < n; ++i)
		f.compute(i);
	ComputeCube c;
	for (int i = 0; i < n; ++i)
		c.compute(i);
}

int main(int argc, char **argv)
{
	if (argc > 1)
		test(atoi(argv[1]));
	return 0;
}
