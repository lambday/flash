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
#include <cstring>
#include <shogun/base/init.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
#include <flash/statistics/internals/DataFetcher.h>
#include <flash/statistics/internals/StreamingDataFetcher.h>
#include <flash/statistics/internals/DataFetcherFactory.h>

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

	std::unique_ptr<DataFetcher> fetcher(DataFetcherFactory::get_instance(feats_p));
	ASSERT(strcmp(fetcher->get_name(), "DataFetcher") == 0);

	CStreamingDenseFeatures<float64_t> *streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	SG_REF(streaming_p);

	std::unique_ptr<DataFetcher> streaming_fetcher(DataFetcherFactory::get_instance(streaming_p));
	ASSERT(strcmp(streaming_fetcher->get_name(), "StreamingDataFetcher") == 0);

	exit_shogun();

	return 0;
}
