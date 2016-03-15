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

#include <shogun/lib/SGMatrix.h>
#include <flash/statistics/internals/ComputationManager.h>
#include <flash/statistics/internals/mmd/UnbiasedFull.h>

using namespace shogun;
using namespace internal;

ComputationManager::ComputationManager()
{
}

ComputationManager::~ComputationManager()
{
}

void ComputationManager::num_data(index_t n)
{
	kernel_matrices.resize(n);
}

SGMatrix<float64_t>& ComputationManager::data(index_t i)
{
	return kernel_matrices[i];
}

template <class Operation> // the operation also has to support gpu
std::vector<typename Operation::return_type> ComputationManager::compute(Operation operation) const
{
	std::vector<typename Operation::return_type> results;

	if (gpu)
	{
		// TODO results = operation.compute_using_gpu(kernel_matrices);
	}
	else
	{
		results.resize(kernel_matrices.size());
#pragma omp parallel for
		for (auto i = 0; i < kernel_matrices.size(); ++i)
		{
			results[i] = operation(kernel_matrices[i]);
		}
	}
	return results;
}

const ComputationManager& ComputationManager::use_gpu()
{
	gpu = true;
	return *this;
}

const ComputationManager& ComputationManager::use_cpu()
{
	gpu = false;
	return *this;
}

template std::vector<typename mmd::UnbiasedFull::return_type> ComputationManager::compute(mmd::UnbiasedFull operation) const;
