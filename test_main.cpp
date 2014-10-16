/*
 * Restructuring Shogun's statistical hypothesis testing framework.
 * Copyright (C) 2014  Soumyajit De
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

#include <iostream>
#include <algorithm>
#include <shogun/base/init.h>
#include <shogun/lib/config.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
#include <flash/statistics/TwoSampleTest.h>
#include <flash/statistics/StreamingTwoSampleTest.h>
#include <flash/statistics/IndependenceTest.h>
#include <flash/statistics/StreamingIndependenceTest.h>

using namespace shogun;

void test1()
{
	std::cout << "test1: two-sample test (without permutation)" << std::endl;
	const index_t dim = 2;
	const index_t num_vec = 4;
	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	statistics::CTwoSampleTest* test = new statistics::CTwoSampleTest();
	test->set_p(feats_p);
	test->set_q(feats_q);

	CFeatures* samples = test->get_samples();
	((CDenseFeatures<float64_t>*)samples)->get_feature_matrix().display_matrix();

	SG_UNREF(test);
	SG_UNREF(samples);
}

void test2()
{
	std::cout << "test2: two-sample test (with permutation)" << std::endl;
	const index_t dim = 2;
	const index_t num_vec = 4;
	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	statistics::CTwoSampleTest* test = new statistics::CTwoSampleTest();
	test->set_p(feats_p);
	test->set_q(feats_q);

	test->set_simulate_h0(true);
	CFeatures* samples = test->get_samples();
	((CDenseFeatures<float64_t>*)samples)->get_feature_matrix().display_matrix();

	SG_UNREF(test);
	SG_UNREF(samples);
}

void test3()
{
	std::cout << "test3: independence test (without permutation)" << std::endl;
	const index_t dim = 2;
	const index_t num_vec = 4;
	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	statistics::CIndependenceTest* test = new statistics::CIndependenceTest();
	test->set_p(feats_p);
	test->set_q(feats_q);

	std::vector<CFeatures*> samples = test->get_samples();
	for (auto feat : samples)
	{
		((CDenseFeatures<float64_t>*)feat)->get_feature_matrix().display_matrix();
		SG_UNREF(feat);
	}

	SG_UNREF(test);
}

void test4()
{
	std::cout << "test4: independence test (with permutation)" << std::endl;
	const index_t dim = 2;
	const index_t num_vec = 4;
	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	statistics::CIndependenceTest* test = new statistics::CIndependenceTest();
	test->set_p(feats_p);
	test->set_q(feats_q);
	test->set_simulate_h0(true);

	std::vector<CFeatures*> samples = test->get_samples();
	for (auto feat : samples)
	{
		((CDenseFeatures<float64_t>*)feat)->get_feature_matrix().display_matrix();
		SG_UNREF(feat);
	}

	SG_UNREF(test);
}

void test5()
{
	std::cout << "test5: streaming two-sample test (without permutation)" << std::endl;
	const index_t dim = 3;
	const index_t num_vec = 8;
	const index_t blocksize = 4;
	const index_t num_blocks_per_burst = 1;

	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	CStreamingFeatures *streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	CStreamingFeatures *streaming_q = new CStreamingDenseFeatures<float64_t>(feats_q);

	statistics::CStreamingTwoSampleTest* test = new statistics::CStreamingTwoSampleTest();
	test->set_p(streaming_p);
	test->set_q(streaming_q);
	test->set_num_samples_p(num_vec);
	test->set_num_samples_q(num_vec);

	streaming_p->start_parser();
	streaming_q->start_parser();

	test->set_blocksize(blocksize);
	test->set_num_blocks_per_burst(num_blocks_per_burst);

	index_t num_bursts = num_vec * 2 / blocksize / num_blocks_per_burst;
	for (index_t i = 0; i < num_bursts; ++i)
	{
		CFeatures* samples = test->get_samples();
		((CDenseFeatures<float64_t>*)samples)->get_feature_matrix().display_matrix();
		SG_UNREF(samples);
	}

	streaming_p->end_parser();
	streaming_q->end_parser();

	SG_UNREF(test);
}

void test6()
{
	std::cout << "test6: streaming two-sample test (with permutation)" << std::endl;
	const index_t dim = 3;
	const index_t num_vec = 8;
	const index_t blocksize = 4;
	const index_t num_blocks_per_burst = 1;

	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	CStreamingFeatures *streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	CStreamingFeatures *streaming_q = new CStreamingDenseFeatures<float64_t>(feats_q);

	statistics::CStreamingTwoSampleTest* test = new statistics::CStreamingTwoSampleTest();
	test->set_p(streaming_p);
	test->set_q(streaming_q);
	test->set_num_samples_p(num_vec);
	test->set_num_samples_q(num_vec);

	streaming_p->start_parser();
	streaming_q->start_parser();

	test->set_blocksize(blocksize);
	test->set_num_blocks_per_burst(num_blocks_per_burst);
	test->set_simulate_h0(true);

	index_t num_bursts = num_vec * 2 / blocksize / num_blocks_per_burst;
	for (index_t i = 0; i < num_bursts; ++i)
	{
		CFeatures* samples = test->get_samples();
		((CDenseFeatures<float64_t>*)samples)->get_feature_matrix().display_matrix();
		SG_UNREF(samples);
	}

	streaming_p->end_parser();
	streaming_q->end_parser();

	SG_UNREF(test);
}

void test7()
{
	std::cout << "test7: streaming two-sample test (without permutation, different num_samples)" << std::endl;
	const index_t dim = 3;
	const index_t num_vec_p = 8;
	const index_t num_vec_q = 12;
	const index_t blocksize = 5;
	const index_t num_blocks_per_burst = 1;

	SGMatrix<float64_t> data_p(dim, num_vec_p);
	SGMatrix<float64_t> data_q(dim, num_vec_q);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec_p, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec_q, dim * num_vec_p);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	CStreamingFeatures *streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	CStreamingFeatures *streaming_q = new CStreamingDenseFeatures<float64_t>(feats_q);

	statistics::CStreamingTwoSampleTest* test = new statistics::CStreamingTwoSampleTest();
	test->set_p(streaming_p);
	test->set_q(streaming_q);
	test->set_num_samples_p(num_vec_p);
	test->set_num_samples_q(num_vec_q);

	streaming_p->start_parser();
	streaming_q->start_parser();

	test->set_blocksize(blocksize);
	test->set_num_blocks_per_burst(num_blocks_per_burst);

	index_t num_bursts = (num_vec_p + num_vec_q) / blocksize / num_blocks_per_burst;
	for (index_t i = 0; i < num_bursts; ++i)
	{
		CFeatures* samples = test->get_samples();
		((CDenseFeatures<float64_t>*)samples)->get_feature_matrix().display_matrix();
		SG_UNREF(samples);
	}

	streaming_p->end_parser();
	streaming_q->end_parser();

	SG_UNREF(test);
}

void test8()
{
	std::cout << "test7: streaming two-sample test (with permutation, different num_samples)" << std::endl;
	const index_t dim = 3;
	const index_t num_vec_p = 8;
	const index_t num_vec_q = 12;
	const index_t blocksize = 5;
	const index_t num_blocks_per_burst = 1;

	SGMatrix<float64_t> data_p(dim, num_vec_p);
	SGMatrix<float64_t> data_q(dim, num_vec_q);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec_p, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec_q, dim * num_vec_p);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	CStreamingFeatures *streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	CStreamingFeatures *streaming_q = new CStreamingDenseFeatures<float64_t>(feats_q);

	statistics::CStreamingTwoSampleTest* test = new statistics::CStreamingTwoSampleTest();
	test->set_p(streaming_p);
	test->set_q(streaming_q);
	test->set_num_samples_p(num_vec_p);
	test->set_num_samples_q(num_vec_q);

	streaming_p->start_parser();
	streaming_q->start_parser();

	test->set_blocksize(blocksize);
	test->set_num_blocks_per_burst(num_blocks_per_burst);
	test->set_simulate_h0(true);

	index_t num_bursts = (num_vec_p + num_vec_q) / blocksize / num_blocks_per_burst;
	for (index_t i = 0; i < num_bursts; ++i)
	{
		CFeatures* samples = test->get_samples();
		((CDenseFeatures<float64_t>*)samples)->get_feature_matrix().display_matrix();
		SG_UNREF(samples);
	}

	streaming_p->end_parser();
	streaming_q->end_parser();

	SG_UNREF(test);
}

void test9()
{
	std::cout << "test9: streaming independence test (without permutation)" << std::endl;
	const index_t dim = 3;
	const index_t num_vec = 8;
	const index_t blocksize = 4;
	const index_t num_blocks_per_burst = 1;

	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	CStreamingFeatures *streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	CStreamingFeatures *streaming_q = new CStreamingDenseFeatures<float64_t>(feats_q);

	statistics::CStreamingIndependenceTest* test = new statistics::CStreamingIndependenceTest();
	test->set_p(streaming_p);
	test->set_q(streaming_q);
	test->set_num_samples_p(num_vec);
	test->set_num_samples_q(num_vec);

	streaming_p->start_parser();
	streaming_q->start_parser();

	test->set_blocksize(blocksize);
	test->set_num_blocks_per_burst(num_blocks_per_burst);

	index_t num_bursts = num_vec * 2 / blocksize / num_blocks_per_burst;
	for (index_t i = 0; i < num_bursts; ++i)
	{
		std::vector<CFeatures*> samples = test->get_samples();
		for (auto s : samples)
		{
			((CDenseFeatures<float64_t>*)s)->get_feature_matrix().display_matrix();
			SG_UNREF(s);
		}
	}

	streaming_p->end_parser();
	streaming_q->end_parser();

	SG_UNREF(test);
}

void test10()
{
	std::cout << "test10: streaming independence test (with permutation)" << std::endl;
	const index_t dim = 3;
	const index_t num_vec = 8;
	const index_t blocksize = 4;
	const index_t num_blocks_per_burst = 1;

	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	CStreamingFeatures *streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	CStreamingFeatures *streaming_q = new CStreamingDenseFeatures<float64_t>(feats_q);

	statistics::CStreamingIndependenceTest* test = new statistics::CStreamingIndependenceTest();
	test->set_p(streaming_p);
	test->set_q(streaming_q);
	test->set_num_samples_p(num_vec);
	test->set_num_samples_q(num_vec);

	streaming_p->start_parser();
	streaming_q->start_parser();

	test->set_blocksize(blocksize);
	test->set_num_blocks_per_burst(num_blocks_per_burst);
	test->set_simulate_h0(true);

	index_t num_bursts = num_vec * 2 / blocksize / num_blocks_per_burst;
	for (index_t i = 0; i < num_bursts; ++i)
	{
		std::vector<CFeatures*> samples = test->get_samples();
		for (auto s : samples)
		{
			((CDenseFeatures<float64_t>*)s)->get_feature_matrix().display_matrix();
			SG_UNREF(s);
		}
	}

	streaming_p->end_parser();
	streaming_q->end_parser();

	SG_UNREF(test);
}

void test11()
{
	std::cout << "test11: streaming independence test (without permutation, different num_samples)" << std::endl;
	const index_t dim = 3;
	const index_t num_vec_p = 8;
	const index_t num_vec_q = 12;
	const index_t blocksize = 5;
	const index_t num_blocks_per_burst = 1;

	SGMatrix<float64_t> data_p(dim, num_vec_p);
	SGMatrix<float64_t> data_q(dim, num_vec_q);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec_p, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec_q, dim * num_vec_p);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	CStreamingFeatures *streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	CStreamingFeatures *streaming_q = new CStreamingDenseFeatures<float64_t>(feats_q);

	statistics::CStreamingIndependenceTest* test = new statistics::CStreamingIndependenceTest();
	test->set_p(streaming_p);
	test->set_q(streaming_q);
	test->set_num_samples_p(num_vec_p);
	test->set_num_samples_q(num_vec_q);

	streaming_p->start_parser();
	streaming_q->start_parser();

	test->set_blocksize(blocksize);
	test->set_num_blocks_per_burst(num_blocks_per_burst);

	index_t num_bursts = (num_vec_p + num_vec_q) / blocksize / num_blocks_per_burst;
	for (index_t i = 0; i < num_bursts; ++i)
	{
		std::vector<CFeatures*> samples = test->get_samples();
		for (auto s : samples)
		{
			((CDenseFeatures<float64_t>*)s)->get_feature_matrix().display_matrix();
			SG_UNREF(s);
		}
	}

	streaming_p->end_parser();
	streaming_q->end_parser();

	SG_UNREF(test);
}

void test12()
{
	std::cout << "test12: streaming independence test (with permutation, different num_samples)" << std::endl;
	const index_t dim = 3;
	const index_t num_vec_p = 8;
	const index_t num_vec_q = 12;
	const index_t blocksize = 5;
	const index_t num_blocks_per_burst = 1;

	SGMatrix<float64_t> data_p(dim, num_vec_p);
	SGMatrix<float64_t> data_q(dim, num_vec_q);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec_p, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec_q, dim * num_vec_p);

	data_p.display_matrix();
	data_q.display_matrix();

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	CStreamingFeatures *streaming_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	CStreamingFeatures *streaming_q = new CStreamingDenseFeatures<float64_t>(feats_q);

	statistics::CStreamingIndependenceTest* test = new statistics::CStreamingIndependenceTest();
	test->set_p(streaming_p);
	test->set_q(streaming_q);
	test->set_num_samples_p(num_vec_p);
	test->set_num_samples_q(num_vec_q);

	streaming_p->start_parser();
	streaming_q->start_parser();

	test->set_blocksize(blocksize);
	test->set_num_blocks_per_burst(num_blocks_per_burst);
	test->set_simulate_h0(true);

	index_t num_bursts = (num_vec_p + num_vec_q) / blocksize / num_blocks_per_burst;
	for (index_t i = 0; i < num_bursts; ++i)
	{
		std::vector<CFeatures*> samples = test->get_samples();
		for (auto s : samples)
		{
			((CDenseFeatures<float64_t>*)s)->get_feature_matrix().display_matrix();
			SG_UNREF(s);
		}
	}

	streaming_p->end_parser();
	streaming_q->end_parser();

	SG_UNREF(test);
}

int main(int argc, char **argv)
{
	init_shogun_with_defaults();
	sg_rand->set_seed(12345);
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();
	test10();
	test11();
	test12();
	exit_shogun();
	return 0;
}
