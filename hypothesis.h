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
