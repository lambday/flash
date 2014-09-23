#ifndef COMPUTE_FOO_H_
#define COMPUTE_FOO_H_

class ComputeFooFunctor
{
public:
	double operator()(double d)
	{
		return d*d;
	}
};

#endif
