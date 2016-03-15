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
#include <algorithm>
#include <shogun/base/init.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/kernel/Kernel.h>
#include <shogun/kernel/GaussianKernel.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
#include <flash/statistics/MMD.h>

using namespace shogun;
using namespace statistics;
using namespace internal;

void test1()
{
	const index_t dim = 3;
	const index_t num_vec = 8;
	const index_t blocksize = 2;
	const index_t num_blocks_per_burst = 2;
	const index_t num_distributions = 2;

	SGMatrix<float64_t> data_p(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	data_p.display_matrix();

	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);
	data_q.display_matrix();

	auto feats_p = new CDenseFeatures<float64_t>(data_p);
	auto feats_q = new CDenseFeatures<float64_t>(data_q);

	auto kernel = new CGaussianKernel();
	kernel->set_width(0.5);

	auto mmd = std::make_unique<CMMD>();
	mmd->set_p(feats_p);
	mmd->set_q(feats_q);
	mmd->set_kernel(kernel);

	auto statistic = mmd->compute_statistic_unbiased_full();
}

int main()
{
	init_shogun_with_defaults();
	test1();
	exit_shogun();
	return 0;
}
