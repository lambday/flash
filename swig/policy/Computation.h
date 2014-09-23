#ifndef COMPUTATION_H_
#define COMPUTATION_H_

#include <ComputeFoo.h>
#include <ComputeCube.h>

template <class Compute>
class Computation
{
public:
	Computation();
	~Computation();
	double compute(double d)
	{
		Compute compute;
		return compute(d);
	}
};

typedef Computation<ComputeFooFunctor> ComputeFoo;
typedef Computation<ComputeCubeFunctor> ComputeCube;

#endif
