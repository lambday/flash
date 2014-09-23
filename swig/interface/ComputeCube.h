#include <Computation.h>

#ifndef COMPUTE_CUBE_H_
#define COMPUTE_CUBE_H_

class ComputeCube : public Computation
{
public:
	ComputeCube() : Computation() {}
	virtual ~ComputeCube() {}
	virtual double compute(double d);
};
#endif
