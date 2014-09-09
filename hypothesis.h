#ifndef HYPOTHESIS_H_
#define HYPOTHESIS_H_

template <class TestData, class Compute>
struct HypothesisTest
{
	inline
	double compute_statistic(Compute compute)
	{
		return compute(test_data);
	}
	TestData test_data;
};

template <class Kernel>
struct ComputeQuadraticTimeMMD
{
	typedef TestData data_type;
	typedef data_type::feat_type feat_type;
	typedef data_type::kernel_type kernel_type;
	enum { statistic_type = QUADRATIC_TIME_MMD };

	template <bool IsPermutationTest>
	double operator()(Kernel* kernel)
	{
		// computation logic
		return 0;
	}
};

class CQuadraticTimeMMD : public CSGObject,
	public HypothesisTest<KernelTwoSampleTestData, ComputeQuadraticTimeMMD>
{
	inline
	double compute_statistic()
	{
		return compute_statistic(ComputeQuadraticTimeMMD<TestData compute());
	}
};

#endif // HYPOTHESIS_H_
