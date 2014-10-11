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
#include <flash/statistics/IndependenceTest.h>

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

int main(int argc, char **argv)
{
	init_shogun_with_defaults();
	test1();
	test2();
	test3();
	test4();
	exit_shogun();
	return 0;
}
