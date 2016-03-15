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

#include <flash/statistics/TwoSampleTest.h>

namespace shogun
{

class CKernel;

namespace statistics
{

class CMMD : public CTwoSampleTest
{
public:
	CMMD();
	virtual ~CMMD();

	float64_t compute_statistic_unbiased_full();
//	float64_t compute_statistic(bool multiple_kernels);

	void use_gpu(bool gpu);
	virtual const char* get_name() const;

private:
	template <class Statistic> float64_t compute_statistic();
	bool use_gpu_for_computation;
};

}

}
#endif // MMD_H_
