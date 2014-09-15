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

template <class Kernel, class TestDataManager>
struct TestKernelManager
{
	using test_type = typename TestDataManager::test_type;

	template <bool IsPermutationTest>
	return_type get_kernel()
	{
	}
	TestDataManager data_manager;
	KernelStore<test_type> store;
};

template <class TestDataManager>
struct kernel_traits
{

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
