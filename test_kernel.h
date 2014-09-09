template <class Kernel, class TestDataManager>
struct TestKernelManager
{
	template <bool IsPermutationTest, bool Precompute>
	Kernel* get_kernel()
	{
	}
};

template <class Features, class Fetcher>
struct TwoSampleTestKernelManager
{
	// it uses get_samples for normal kernels, and uses custom kernel otherwise
	template <bool IsPermutationTest>
	Kernel* get_kernel()
	{
		if (kernel->get_kernel_type() == K_CUSTOM)
		{
			init_kernel<IsPermutationTest, true>();
			CList* samples = get_samples<IsPermutationTest>();
			Features* feats_p = samples->get_first_element();
			Features* feats_q = samples->get_next_element();
			Features* feats_p_and_q = feats_p->create_merged_copy(feats_q);
			kernel->init(feats_p_and_q, feats_p_and_q);
		}
		else
		{
			init_kernel<IsPermutationTest, false>();
		}
		return kernel;
	}
	// is it okay if I always precompute the kernel? because even for block tests
	// get_kernel() will be called many times and
	// recipe : if permutation test, always precompute the kernel
	// otherwise just keep it as it is
	template <bool IsPermutationTest, bool IsCustomKernel>
	void init_kernel()
	{
	}
	Kernel* kernel;
};

// similar to above, except that get_kernels() should return a list of kernels
template <class Kernel>
struct IndependenceTestKernel
{
};

