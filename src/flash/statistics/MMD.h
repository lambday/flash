/*
 * Restructuring Shogun's statistical hypothesis testing framework.
 * Copyright (C) 2016  Soumyajit De
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

#ifndef MMD_H_
#define MMD_H_

#include <map>
#include <flash/statistics/TwoSampleTest.h>

namespace shogun
{

class CKernel;

namespace statistics
{

enum class S_TYPE
{
	S_UNBIASED_FULL,
	S_UNBIASED_INCOMPLETE,
	S_BIASED
};

enum class V_EST_METHOD
{
	V_DIRECT,
	V_PERMUTATION
};

class CMMD : public CTwoSampleTest
{
public:
	CMMD();
	virtual ~CMMD();

	float64_t compute_statistic();
	virtual float64_t compute_variance();
	// ^ overridden by subclasses with appropriate normalization
	// constant

//	float64_t compute_statistic(bool multiple_kernels);

	void use_gpu(bool gpu);

	void set_simulate_h0(bool h0);

	void set_statistic_type(S_TYPE stype);

	// make sure that quadratic time mmd does not set permutation method
	void set_variance_estimation_method(V_EST_METHOD vmethod);

	void set_num_null_samples(index_t null_samples);
	SGVector<float64_t> sample_null();

	virtual const char* get_name() const;

protected:
	std::pair<SGVector<float64_t>, SGVector<float64_t>> compute_statistic_variance();
	bool use_gpu_for_computation;
	bool simulate_h0;
	index_t num_null_samples;
	S_TYPE statistic_type;
	V_EST_METHOD variance_estimation_method;
};

}

}
#endif // MMD_H_
