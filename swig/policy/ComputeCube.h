#ifndef COMPUTE_CUBE_H_
#define COMPUTE_CUBE_H_

class ComputeCubeFunctor
{
public:
	double operator()(double d)
	{
		return d*d*d;
	}
};

#endif
