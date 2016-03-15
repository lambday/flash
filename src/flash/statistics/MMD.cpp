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

#include <vector>
#include <memory>
#include <iostream>
#include <shogun/kernel/Kernel.h>
#include <shogun/features/Features.h>
#include <flash/statistics/MMD.h>
#include <flash/statistics/internals/NextSamples.h>
#include <flash/statistics/internals/DataManager.h>
#include <flash/statistics/internals/KernelManager.h>
#include <flash/statistics/internals/ComputationManager.h>
#include <flash/statistics/internals/mmd/UnbiasedFull.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

CMMD::CMMD() : CTwoSampleTest(), use_gpu_for_computation(false)
{
}

CMMD::~CMMD()
{
}

float64_t CMMD::compute_statistic_unbiased_full()
{
	return compute_statistic<mmd::UnbiasedFull>();
}

template <class Statistic>
float64_t CMMD::compute_statistic()
{
	ComputationManager cm;
	DataManager& dm = get_data_manager();
	const KernelManager& km = get_kernel_manager();

	dm.start();
	auto next_burst = dm.next();

	auto num_samples_p = 0;
	float64_t statistic = 0;

	while (!next_burst.empty())
	{
		cm.num_data(next_burst.num_blocks());
		for (auto i = 0; i < next_burst.num_blocks(); ++i)
		{
			auto block_p = next_burst[0][i];
			auto block_q = next_burst[1][i];

			num_samples_p = block_p->get_num_vectors();

			auto block_p_q = block_p->create_merged_copy(block_q.get());
			if (simulate_h0)
			{
				SGVector<index_t> inds(block_p_q->get_num_vectors());
				std::iota(inds.vector, inds.vector + inds.vlen, 0);
				CMath::permute(inds);
				block_p_q->add_subset(inds);
			}

			block_p = nullptr;
			block_q = nullptr;

			try
			{
				km.kernel_at(0)->init(block_p_q, block_p_q);
				get_kernel_manager().precompute_kernel_at(0);
				cm.data(i) = km.kernel_at(0)->get_kernel_matrix();
				get_kernel_manager().restore_kernel_at(0);
				km.kernel_at(0)->remove_lhs_and_rhs();
			}
			catch (ShogunException e)
			{
				std::cerr << e.get_exception_string() << std::endl;
				std::cerr << "Try using less number of blocks per burst" << std::endl;
			}
		}

		std::vector<typename Statistic::return_type> mmds;

		if (use_gpu_for_computation)
		{
			mmds = cm.use_gpu().compute(Statistic(num_samples_p));
		}
		else
		{
			mmds = cm.use_cpu().compute(Statistic(num_samples_p));
		}

		// TODO averaging of the mmds logic does here

		next_burst = dm.next();
	}

	dm.end();

	return statistic;
}

void CMMD::use_gpu(bool gpu)
{
	use_gpu_for_computation = gpu;
}

void CMMD::set_simulate_h0(bool h0)
{
	simulate_h0 = h0;
}

const char* CMMD::get_name() const
{
	return "MMD";
}
