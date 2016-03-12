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
#include <shogun/lib/SGMatrix.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
#include <flash/statistics/internals/DataManager.h>
#include <flash/statistics/internals/NextSamples.h>

using namespace shogun;
using namespace internal;

void test1()
{
	const index_t dim = 3;
	const index_t num_vec = 8;
	const index_t blocksize = 2;
	const index_t num_blocks_per_burst = 2;
	const index_t num_distributions = 1;

	SGMatrix<float64_t> data_p(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	data_p.display_matrix();

	using feat_type = CDenseFeatures<float64_t>;
	auto feats_p = new feat_type(data_p);

	DataManager mgr(num_distributions);
	mgr.samples_at(0) = feats_p;
	mgr.start();
	auto nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}

	mgr.reset();
	mgr.set_blocksize(blocksize);
	mgr.set_num_blocks_per_burst(num_blocks_per_burst);

	nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}
}

void test2()
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

	using feat_type = CDenseFeatures<float64_t>;
	auto feats_p = new feat_type(data_p);
	auto feats_q = new feat_type(data_q);

	DataManager mgr(num_distributions);
	mgr.samples_at(0) = feats_p;
	mgr.samples_at(1) = feats_q;
	mgr.start();
	auto nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		tmp = static_cast<feat_type*>(nxt_samples[1].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}

	mgr.reset();
	mgr.set_blocksize(blocksize);
	mgr.set_num_blocks_per_burst(num_blocks_per_burst);

	nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		tmp = static_cast<feat_type*>(nxt_samples[1].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}
}

void test3()
{
	const index_t dim = 3;
	const index_t num_vec_p = 8;
	const index_t num_vec_q = 12;
	const index_t blocksize = 5;
	const index_t num_blocks_per_burst = 1;
	const index_t num_distributions = 2;

	SGMatrix<float64_t> data_p(dim, num_vec_p);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec_p, 0);
	data_p.display_matrix();

	SGMatrix<float64_t> data_q(dim, num_vec_q);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec_q, dim * num_vec_p);
	data_q.display_matrix();

	using feat_type = CDenseFeatures<float64_t>;
	auto feats_p = new feat_type(data_p);
	auto feats_q = new feat_type(data_q);

	DataManager mgr(num_distributions);
	mgr.samples_at(0) = feats_p;
	mgr.samples_at(1) = feats_q;
	mgr.start();
	auto nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		tmp = static_cast<feat_type*>(nxt_samples[1].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}

	mgr.reset();
	mgr.set_blocksize(blocksize);
	mgr.set_num_blocks_per_burst(num_blocks_per_burst);

	nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		tmp = static_cast<feat_type*>(nxt_samples[1].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}
}

void test4()
{
	const index_t dim = 3;
	const index_t num_vec = 8;
	const index_t blocksize = 2;
	const index_t num_blocks_per_burst = 2;
	const index_t num_distributions = 1;

	SGMatrix<float64_t> data_p(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	data_p.display_matrix();

	using feat_type = CDenseFeatures<float64_t>;
	auto feats_p = new feat_type(data_p);
	auto streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	SG_REF(streaming_p);

	DataManager mgr(num_distributions);
	mgr.samples_at(0) = streaming_p;
	mgr.num_samples_at(0) = num_vec;

	mgr.start();
	auto nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}

	mgr.reset();
	mgr.set_blocksize(blocksize);
	mgr.set_num_blocks_per_burst(num_blocks_per_burst);

	nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}
}

void test5()
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

	using feat_type = CDenseFeatures<float64_t>;
	auto feats_p = new feat_type(data_p);
	auto streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	SG_REF(streaming_p);
	auto feats_q = new feat_type(data_q);
	auto streaming_q = new CStreamingDenseFeatures<float64_t>(feats_q);
	SG_REF(streaming_q);

	DataManager mgr(num_distributions);
	mgr.samples_at(0) = streaming_p;
	mgr.samples_at(1) = streaming_q;
	mgr.num_samples_at(0) = num_vec;
	mgr.num_samples_at(1) = num_vec;

	mgr.start();
	auto nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		tmp = static_cast<feat_type*>(nxt_samples[1].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}

	mgr.reset();
	mgr.set_blocksize(blocksize);
	mgr.set_num_blocks_per_burst(num_blocks_per_burst);

	nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		tmp = static_cast<feat_type*>(nxt_samples[1].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}
}

void test6()
{
	const index_t dim = 3;
	const index_t num_vec_p = 8;
	const index_t num_vec_q = 12;
	const index_t blocksize = 5;
	const index_t num_blocks_per_burst = 1;
	const index_t num_distributions = 2;

	SGMatrix<float64_t> data_p(dim, num_vec_p);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec_p, 0);
	data_p.display_matrix();

	SGMatrix<float64_t> data_q(dim, num_vec_q);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec_q, dim * num_vec_p);
	data_q.display_matrix();

	using feat_type = CDenseFeatures<float64_t>;
	auto feats_p = new feat_type(data_p);
	auto streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	SG_REF(streaming_p);
	auto feats_q = new feat_type(data_q);
	auto streaming_q = new CStreamingDenseFeatures<float64_t>(feats_q);
	SG_REF(streaming_q);

	DataManager mgr(num_distributions);
	mgr.samples_at(0) = streaming_p;
	mgr.samples_at(1) = streaming_q;
	mgr.num_samples_at(0) = num_vec_p;
	mgr.num_samples_at(1) = num_vec_q;

	mgr.start();
	auto nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		tmp = static_cast<feat_type*>(nxt_samples[1].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}

	mgr.reset();
	mgr.set_blocksize(blocksize);
	mgr.set_num_blocks_per_burst(num_blocks_per_burst);

	nxt_samples = mgr.next();
	while (!nxt_samples.empty())
	{
		auto tmp = static_cast<feat_type*>(nxt_samples[0].get());
		tmp->get_feature_matrix().display_matrix();
		tmp = static_cast<feat_type*>(nxt_samples[1].get());
		tmp->get_feature_matrix().display_matrix();
		nxt_samples = mgr.next();
	}
}
int main()
{
	init_shogun_with_defaults();

	test1();
	test2();
	test3();

	test4();
	test4();
	test6();

	exit_shogun();
	return 0;
}
