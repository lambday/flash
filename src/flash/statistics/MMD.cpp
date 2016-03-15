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

			// TODO permutation logic goes here
			auto block_p_q = std::shared_ptr<CFeatures>(block_p->create_merged_copy(block_q.get()),
					[](auto& ptr) { SG_UNREF(ptr); });

			block_p = nullptr;
			block_q = nullptr;

			km.kernel_at(0)->init(block_p_q.get(), block_p_q.get());
			get_kernel_manager().precompute_kernel_at(0);

			block_p_q = nullptr;

			cm.data(i) = km.kernel_at(0)->get_kernel_matrix();
		}

		std::vector<typename mmd::UnbiasedFull::return_type> mmds;

		if (use_gpu_for_computation)
		{
			mmds = cm.use_gpu().compute(mmd::UnbiasedFull(num_samples_p));
		}
		else
		{
			mmds = cm.use_cpu().compute(mmd::UnbiasedFull(num_samples_p));
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

const char* CMMD::get_name() const
{
	return "MMD";
}
