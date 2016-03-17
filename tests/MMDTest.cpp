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
#include <flash/statistics/QuadraticTimeMMD.h>

using namespace shogun;
using namespace statistics;
using namespace internal;

void test1(CFeatures* feats_p, CFeatures* feats_q, CKernel* kernel)
{
	auto mmd = std::make_unique<CQuadraticTimeMMD>();
	mmd->set_p(feats_p);
	mmd->set_q(feats_q);
	mmd->set_kernel(kernel);

	mmd->set_statistic_type(S_TYPE::UNBIASED_FULL);

	auto statistic = mmd->compute_statistic();
	std::cout << statistic << std::endl;

	auto variance = mmd->compute_variance();
	std::cout << variance << std::endl;

	mmd->set_statistic_type(S_TYPE::UNBIASED_INCOMPLETE);

	statistic = mmd->compute_statistic();
	std::cout << statistic << std::endl;

	variance = mmd->compute_variance();
	std::cout << variance << std::endl;

	mmd->set_statistic_type(S_TYPE::BIASED_FULL);

	statistic = mmd->compute_statistic();
	std::cout << statistic << std::endl;

	variance = mmd->compute_variance();
	std::cout << variance << std::endl;
}

int main()
{
	init_shogun_with_defaults();

	const index_t dim = 2;
	const index_t num_vec = 6;

	SGMatrix<float64_t> data_p(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 2);
	std::for_each(data_p.matrix, data_p.matrix + dim * num_vec, [](auto& v) { v = sin(v); });
	data_p.display_matrix();

	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, 2);
	std::for_each(data_q.matrix, data_q.matrix + dim * num_vec, [](auto& v) { v = log(v); });
	data_q.display_matrix();

	auto feats_p = new CDenseFeatures<float64_t>(data_p);
	auto feats_q = new CDenseFeatures<float64_t>(data_q);

	auto kernel = new CGaussianKernel();
	kernel->set_width(0.5);

	test1(feats_p, feats_q, kernel);

	exit_shogun();
	return 0;
}
