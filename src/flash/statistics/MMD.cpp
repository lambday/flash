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
#include <shogun/kernel/CustomKernel.h>
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

CMMD::CMMD() : CTwoSampleTest(), use_gpu_for_computation(false),
simulate_h0(false), statistic_type(S_TYPE::S_UNBIASED_FULL), num_null_samples(0)
{
}

CMMD::~CMMD()
{
}

float64_t CMMD::compute_statistic()
{
	switch (statistic_type)
	{
		case S_TYPE::S_UNBIASED_FULL: return compute_statistic_variance<mmd::UnbiasedFull>();
		default : return 0 ; // TODO write some error msg
	};
}

template <class Statistic>
float64_t CMMD::compute_statistic_variance()
{
	ComputationManager cm;
	DataManager& dm = get_data_manager();
	const KernelManager& km = get_kernel_manager();

	auto num_samples_p = 0;
	float64_t statistic = 0;
	auto term_counter = 1;

	dm.start();
	auto next_burst = dm.next();

	while (!next_burst.empty())
	{
		cm.num_data(next_burst.num_blocks());
#pragma omp parallel for
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
				auto kernel = std::unique_ptr<CKernel>(static_cast<CKernel*>(km.kernel_at(0)->clone()));
				kernel->init(block_p_q, block_p_q);
				cm.data(i) = std::make_unique<CCustomKernel>(kernel.get())->get_kernel_matrix();
				kernel->remove_lhs_and_rhs();
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

		for (auto i = 0; i < mmds.size(); ++i)
		{
			auto delta = mmds[i] - statistic;
			statistic += delta / term_counter++;
		}

		next_burst = dm.next();
	}

	dm.end();

	return statistic;
}

SGVector<float64_t> CMMD::sample_null()
{
	SGVector<float64_t> null_samples(num_null_samples);
	auto old = simulate_h0;
	simulate_h0 = true;
	for (auto i = 0; i < num_null_samples; ++i)
	{
		null_samples[i] = compute_statistic();
	}
	simulate_h0 = old;
	return null_samples;
}

void CMMD::set_num_null_samples(index_t null_samples)
{
	num_null_samples = null_samples;
}

void CMMD::use_gpu(bool gpu)
{
	use_gpu_for_computation = gpu;
}

void CMMD::set_simulate_h0(bool h0)
{
	simulate_h0 = h0;
}

void CMMD::set_statistic_type(S_TYPE stype)
{
	statistic_type = stype;
}

const char* CMMD::get_name() const
{
	return "MMD";
}
