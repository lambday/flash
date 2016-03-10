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

#include <flash/statistics/internals/StreamingDataFetcher.h>
#include <shogun/base/init.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
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
	const index_t blocksize = 2;
	const index_t num_blocks_per_burst = 2;

	SGMatrix<float64_t> data_p(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	data_p.display_matrix();

	using feat_type = CDenseFeatures<float64_t>;
	auto feats_p = new feat_type(data_p);
	CStreamingFeatures *streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	SG_REF(streaming_p); // TODO check why this refcount is required

	StreamingDataFetcher fetcher(streaming_p);
	fetcher.set_num_samples(num_vec);

	fetcher.start();
	auto curr = fetcher.next();
	while (curr != nullptr)
	{
		auto tmp = static_cast<feat_type*>(curr.get());
		tmp->get_feature_matrix().display_matrix();
		curr = fetcher.next();
	}

	fetcher.reset();
	fetcher.fetch_blockwise().with_blocksize(blocksize).with_num_blocks_per_burst(num_blocks_per_burst);

	fetcher.start();
	curr = fetcher.next();
	while (curr != nullptr)
	{
		auto tmp = static_cast<feat_type*>(curr.get());
		tmp->get_feature_matrix().display_matrix();
		curr = fetcher.next();
	}

	fetcher.end();
/*
	NextSamples next_samples(num_distributions);
	ASSERT(next_samples.empty());

	next_samples.at(0) = std::shared_ptr<CFeatures>(feats_p, [](auto& ptr) { SG_UNREF(ptr); });
	next_samples.at(1) = std::shared_ptr<CFeatures>(feats_q, [](auto& ptr) { SG_UNREF(ptr); });

	ASSERT(next_samples.empty() == false);
	ASSERT(feats_p == next_samples.get(0).get());
	ASSERT(feats_q == next_samples.get(1).get());

	next_samples.at(0) = nullptr;
	ASSERT(next_samples.empty());
*/
	exit_shogun();

	return 0;
}
