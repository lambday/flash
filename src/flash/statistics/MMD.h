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

#include <memory>
#include <flash/statistics/TwoSampleTest.h>

namespace shogun
{

class CKernel;
template <typename T> class SGVector;

namespace statistics
{

enum class S_TYPE
{
	UNBIASED_FULL,
	UNBIASED_INCOMPLETE,
	BIASED_FULL
};

enum class V_METHOD
{
	DIRECT,
	PERMUTATION
};

template <class Derived>
class CMMD : public CTwoSampleTest
{
public:
	CMMD();
	virtual ~CMMD();

	float64_t compute_statistic();
	SGVector<float64_t> compute_statistic(bool multiple_kernels);

	float64_t compute_variance();
	SGVector<float64_t> compute_variance(bool multiple_kernels);

	void set_statistic_type(S_TYPE stype);
	void set_variance_estimation_method(V_METHOD vmethod);

	void set_simulate_null(bool null);
	void set_num_null_samples(index_t null_samples);
	SGVector<float64_t> sample_null();

	void use_gpu(bool gpu);

	virtual const char* get_name() const;
private:
	struct Self;
	std::unique_ptr<Self> self;

};

}

}
#endif // MMD_H_
