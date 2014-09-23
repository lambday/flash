#include <cstdlib>
#include <ComputeFoo.h>
#include <ComputeCube.h>

void test(int n)
{
	Computation *f = new ComputeFoo();
	for (int i = 0; i < n; ++i)
		f->compute(i);
	delete f;
	f = new ComputeCube();
	for (int i = 0; i < n; ++i)
		f->compute(i);
	delete f;
}

int main(int argc, char **argv)
{
	if (argc > 1)
		test(atoi(argv[1]));
	return 0;
}
