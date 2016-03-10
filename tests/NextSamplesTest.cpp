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

#include <flash/statistics/internals/NextSamples.h>
#include <shogun/base/init.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <vector>
#include <memory>
#include <algorithm>

using namespace shogun;
using namespace internal;

int main()
{
	init_shogun_with_defaults();

	const index_t dim = 3;
	const index_t num_vec = 8;
	const index_t num_distributions = 2;

	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	using feat_type = CDenseFeatures<float64_t>;
	auto feats_p = new feat_type(data_p);
	SG_REF(feats_p);
	auto feats_q = new feat_type(data_q);
	SG_REF(feats_q);

	NextSamples next_samples(num_distributions);
	ASSERT(next_samples.empty());

	next_samples.at(0) = std::shared_ptr<CFeatures>(feats_p, [](auto& ptr) { SG_UNREF(ptr); });
	next_samples.at(1) = std::shared_ptr<CFeatures>(feats_q, [](auto& ptr) { SG_UNREF(ptr); });

	ASSERT(next_samples.empty() == false);
	ASSERT(feats_p == next_samples.get(0).get());
	ASSERT(feats_q == next_samples.get(1).get());

	next_samples.at(0) = nullptr;
	ASSERT(next_samples.empty());

	exit_shogun();

	return 0;
}
