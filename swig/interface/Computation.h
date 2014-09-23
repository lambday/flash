#ifndef COMPUTE_SQUARE_H_
#define COMPUTE_SQUARE_H_

class Computation
{
public:
	Computation() {}
	virtual ~Computation() {}
	virtual double compute(double d) = 0;
};
#endif
