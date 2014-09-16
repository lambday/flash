/*
 * Restructuring Shogun's statistical hypothesis testing framework.
 * Copyright (C) 2014  Soumyajit De
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <test_data.h>
#include <shogun/kernel/Kernel.h>

using namespace shogun;

// DESIGN NOTES:
// ingrinding feature type for tests may be wise since we know which feature
// type is being used for a particular test (as in, for a particular CSGObject* wrapper)
// but we shouldn't do the same for kernels sincekernels are in the application at the
// runtime. so relying on kernel-type at the compilation phase is not something we should do.

// or maybe we can, if we explicitly instantiate only a chosen few types of
// kernels in the cpp.

// for example, we can use concrete CDenseFeatures or CSparseFeatures in the
// CHSIC, CQuadraticTimeMMD and concrete CStreamingDenseFeatures or
// CStreamingSparseFeatures in the CStreamingMMD or CStreamingHSIC tests but
// there is no way to know the kernel type in compile time (of the library).
// It's better we work with a factory like component and rely on runtime polymorphism.

// or one thing we can do, we can move the test data manager and test kernel manager
// compilation part in the application program (these are small components so doesn't
// take long since not all types of features will get instantiated during compilation
// but only the ones that are actually used). But then the CSGObject* wrappers have to work
// with a non-template base pointer of the data and kernel manager which is impossible.

// if two-sample test, return one kernel
// if independence test, return n kernels where n is the number of distributions
// in case of multiple kernels, return one by one, precomputed
// also note that we don't need data manager if custom kernel is provided

// NOTE: cannot be templated on Kernel because it can be anything and it really
// depends on runtime. We don't want to bloat up the binary size by making compiler
// produce code for each of the kernel types. Here we should really work with base CKernel.

// invariant :- get_kernel should always return CCustomKernels.
// if CCombinedKernels are not used:
// 		for two-sample test, it just returns one precomputed kernels
//		for independence test, it just returns n precomputed kernels where n is the number of distributions
// else
//		for two-sample test, it returns N precomputed kernels one by one
//		for independence test, it returns nxN precomputed kernels, n at a time
// This simplifies our design: depending on the test_type, we return things.
// in case CCombinedKernels are used, we return the same thing multiple times.

template <class Kernel, class TestDataManager>
struct TestKernelManager
{
	using test_type = typename TestDataManager::test_type;

	/*
	template <bool IsPermutationTest>
	return_type get_kernel()
	{
	}
	*/
	TestDataManager data_manager;

	// having this functorized because we want to provide get_kernel() i.e. without param
	// for single kernels and get_kernel(i) i.e. with kernel index for combined kernel
	// without having to write two different methods here. Method overloading could have
	// served the purpose but then we have to disable one or the other based on the
	// kernel type in the template specialization of TestKernelManager which we don't want
	GetKernelFunctor<Kernel> get_kernel;

	//KernelStore<test_type> store;
};

template <Kernel>
struct GetKernelFunctor
{
};

template <>
struct GetKernelFunctor<CCombinedKernel>
{
};

template <class Kernel, class TestType> struct kernel_traits;

template <class Kernel> template <class Features>
struct kernel_traits <Kernel, TwoSampleTest<Features> >
{
	using return_type = Kernel*;
	// gotta check with compatibility for CustomKernel
};

template <class Kernel> template <class Features>
struct kernel_traits <Kernel, IndependenceTest<Features> >
{
	using return_type = std::vector<Kernel*>;
	// gotta check with compatibility for CustomKernel
};

/*
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
*/
