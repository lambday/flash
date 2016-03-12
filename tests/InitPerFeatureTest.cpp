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
#include <algorithm>
#include <shogun/base/init.h>
#include <shogun/lib/common.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <flash/statistics/internals/DataManager.h>

using namespace shogun;
using namespace internal;

void test1()
{
	const index_t dim = 3;
	const index_t num_vec = 8;
	const index_t num_distributions = 1;

	SGMatrix<float64_t> data_p(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	data_p.display_matrix();

	using feat_type = CDenseFeatures<float64_t>;
	auto feats_p = new feat_type(data_p);

	DataManager mgr(num_distributions);

	// checking non const samples_at
	mgr.samples_at(0) = feats_p;
	auto stored_feats = mgr.samples_at(0);
	ASSERT(feats_p == stored_feats);

	// checking const samples_at
	const DataManager& cmgr = mgr;
	auto stored_feats2 = cmgr.samples_at(0);
	ASSERT(feats_p == stored_feats2);
}

int main()
{
	init_shogun_with_defaults();
	test1();
	exit_shogun();
	return 0;
}
