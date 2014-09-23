#include <Computation.h>

#ifndef COMPUTE_FOO_H_
#define COMPUTE_FOO_H_

class ComputeFoo : public Computation
{
public:
	ComputeFoo() : Computation() {}
	virtual ~ComputeFoo() {}
	virtual double compute(double d);
};
#endif
