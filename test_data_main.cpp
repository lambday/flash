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

#include <test_data.h>
#include <shogun/base/init.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/features/streaming/generators/MeanShiftDataGenerator.h>
#include <algorithm>
#include <iostream>

using namespace shogun;
using namespace std;

template <bool IsPermutationTest>
void independence_test_get_all()
{
	const index_t num_feats = 3;
	const index_t num_vec = 8;
	SGMatrix<float64_t> data_p(num_feats, num_vec);
	SGMatrix<float64_t> data_q(num_feats, num_vec);
	std::iota(data_p.matrix, data_p.matrix + num_feats * num_vec, 1);
	std::iota(data_q.matrix, data_q.matrix + num_feats * num_vec, data_p.matrix[num_feats*num_vec-1]+1);

	using feats_type = CDenseFeatures<float64_t>;
	feats_type* feats_p = new feats_type(data_p);
	feats_type* feats_q = new feats_type(data_q);

	TestDataManager<feats_type, FetchAll, IndependenceTest> data_manager;
	data_manager.push_back(feats_p);
	data_manager.push_back(feats_q);
	data_manager.set_permutation(IsPermutationTest);

	std::vector<feats_type*> samples = data_manager.get_samples(FetchAll<feats_type>());
	for (auto sample : samples)
		sample->get_feature_matrix().display_matrix();
}

template <bool IsPermutationTest>
void two_sample_test_get_all()
{
	const index_t num_feats = 3;
	const index_t num_vec = 8;
	SGMatrix<float64_t> data_p(num_feats, num_vec);
	SGMatrix<float64_t> data_q(num_feats, num_vec);
	std::iota(data_p.matrix, data_p.matrix + num_feats * num_vec, 1);
	std::iota(data_q.matrix, data_q.matrix + num_feats * num_vec, data_p.matrix[num_feats*num_vec-1]+1);

	using feats_type = CDenseFeatures<float64_t>;
	feats_type* feats_p = new feats_type(data_p);
	feats_type* feats_q = new feats_type(data_q);

	TestDataManager<feats_type, FetchAll, TwoSampleTest> data_manager;
	data_manager.push_back(feats_p);
	data_manager.push_back(feats_q);
	data_manager.set_permutation(IsPermutationTest);

	feats_type* samples = data_manager.get_samples(FetchAll<feats_type>());
	samples->get_feature_matrix().display_matrix();
	SG_UNREF(samples);
}

template <bool IsPermutationTest>
void independence_test_get_blocks()
{
	const index_t num_feats = 3;
	const index_t num_vec = 8;
	SGMatrix<float64_t> data_p(num_feats, num_vec);
	SGMatrix<float64_t> data_q(num_feats, num_vec);
	std::iota(data_p.matrix, data_p.matrix + num_feats * num_vec, 1);
	std::iota(data_q.matrix, data_q.matrix + num_feats * num_vec, data_p.matrix[num_feats*num_vec-1]+1);

	int blocksize = 4;
	int num_blocks_per_burst = 1;

	using feats_type = CDenseFeatures<float64_t>;
	using streaming_feats_type = CStreamingDenseFeatures<float64_t>;
	using return_type = fetch_traits<streaming_feats_type>::return_type;

	feats_type* feats_p = new feats_type(data_p);
	feats_type* feats_q = new feats_type(data_q);

	TestDataManager<streaming_feats_type, FetchBlocks, IndependenceTest> data_manager;
	data_manager.push_back(new streaming_feats_type(feats_p));
	data_manager.push_back(new streaming_feats_type(feats_q));
	data_manager.set_permutation(IsPermutationTest);

	int num_burst = num_vec / blocksize / num_blocks_per_burst;
	for (int burst = 0; burst < num_burst; ++burst)
	{
		std::cout << "burst : " << burst << std::endl;
		FetchBlocks<streaming_feats_type> fetch_functor(blocksize, num_blocks_per_burst);
		std::vector<return_type*> samples = data_manager.get_samples(fetch_functor);
		for (auto sample : samples)
		{
			sample->get_feature_matrix().display_matrix();
			SG_UNREF(sample);
		}
	}
}

template <bool IsPermutationTest>
void two_sample_test_get_blocks()
{
	const index_t num_feats = 3;
	const index_t num_vec = 8;
	SGMatrix<float64_t> data_p(num_feats, num_vec);
	SGMatrix<float64_t> data_q(num_feats, num_vec);
	std::iota(data_p.matrix, data_p.matrix + num_feats * num_vec, 1);
	std::iota(data_q.matrix, data_q.matrix + num_feats * num_vec, data_p.matrix[num_feats*num_vec-1]+1);

	int blocksize = 4;
	int num_blocks_per_burst = 1;

	using feats_type = CDenseFeatures<float64_t>;
	using streaming_feats_type = CStreamingDenseFeatures<float64_t>;
	using return_type = fetch_traits<streaming_feats_type>::return_type;

	feats_type* feats_p = new feats_type(data_p);
	feats_type* feats_q = new feats_type(data_q);

	TestDataManager<streaming_feats_type, FetchBlocks, TwoSampleTest> data_manager;
	data_manager.push_back(new streaming_feats_type(feats_p));
	data_manager.push_back(new streaming_feats_type(feats_q));
	data_manager.set_permutation(IsPermutationTest);

	int num_burst = num_vec / blocksize / num_blocks_per_burst;
	for (int burst = 0; burst < num_burst; ++burst)
	{
		std::cout << "burst : " << burst << std::endl;
		FetchBlocks<streaming_feats_type> fetch_functor(blocksize, num_blocks_per_burst);
		return_type* samples = data_manager.get_samples(fetch_functor);
		samples->get_feature_matrix().display_matrix();
		SG_UNREF(samples);
	}
}

template <bool IsPermutationTest>
void independence_test_get_blocks_from_generator()
{
	const index_t num_feats = 3;
	const index_t num_vec = 4;
	float64_t difference = 0.5;
	int blocksize = 2;
	int num_blocks_per_burst = 1;

	using feats_type = CStreamingDenseFeatures<float64_t>;
	using return_type = fetch_traits<feats_type>::return_type;
	feats_type* feats_p = new CMeanShiftDataGenerator(0, num_feats);
	feats_type* feats_q = new CMeanShiftDataGenerator(difference, num_feats);

	TestDataManager<feats_type, FetchBlocks, IndependenceTest> data_manager;
	data_manager.push_back(feats_p);
	data_manager.push_back(feats_q);
	data_manager.set_permutation(IsPermutationTest);

	int num_burst = num_vec / blocksize / num_blocks_per_burst;
	for (int burst = 0; burst < num_burst; ++burst)
	{
		std::cout << "burst : " << burst << std::endl;
		FetchBlocks<feats_type> fetch_functor(blocksize, num_blocks_per_burst);
		std::vector<return_type*> samples = data_manager.get_samples(fetch_functor);
		for (auto sample : samples)
		{
			sample->get_feature_matrix().display_matrix();
			SG_UNREF(sample);
		}
	}
}

template <bool IsPermutationTest>
void two_sample_test_get_blocks_from_generator()
{
	const index_t num_feats = 3;
	const index_t num_vec = 4;
	float64_t difference = 0.5;
	int blocksize = 2;
	int num_blocks_per_burst = 1;

	using feats_type = CStreamingDenseFeatures<float64_t>;
	using return_type = fetch_traits<feats_type>::return_type;
	feats_type* feats_p = new CMeanShiftDataGenerator(0, num_feats);
	feats_type* feats_q = new CMeanShiftDataGenerator(difference, num_feats);

	TestDataManager<feats_type, FetchBlocks, TwoSampleTest> data_manager;
	data_manager.push_back(feats_p);
	data_manager.push_back(feats_q);
	data_manager.set_permutation(IsPermutationTest);

	int num_burst = num_vec / blocksize / num_blocks_per_burst;
	for (int burst = 0; burst < num_burst; ++burst)
	{
		std::cout << "burst : " << burst << std::endl;
		FetchBlocks<feats_type> fetch_functor(blocksize, num_blocks_per_burst);
		return_type* samples = data_manager.get_samples(fetch_functor);
		samples->get_feature_matrix().display_matrix();
		SG_UNREF(samples);
	}
}

void test1()
{
	std::cout << "[independence test] fetch all feats with permutation" << std::endl;
	independence_test_get_all<true>();
	std::cout << "[independence test] fetch all feats without permutation" << std::endl;
	independence_test_get_all<false>();
	std::cout << "[two-sample test] fetch all feats with permutation" << std::endl;
	two_sample_test_get_all<true>();
	std::cout << "[two-sample test] fetch all feats without permutation" << std::endl;
	two_sample_test_get_all<false>();
}

void test2()
{
	std::cout << "[independence test] fetch blocks with permutation" << std::endl;
	independence_test_get_blocks<true>();
	std::cout << "[independence test] fetch blocks without permutation" << std::endl;
	independence_test_get_blocks<false>();
	std::cout << "[two-sample test] fetch blocks with permutation" << std::endl;
	two_sample_test_get_blocks<true>();
	std::cout << "[two-sample test] fetch blocks without permutation" << std::endl;
	two_sample_test_get_blocks<false>();
}

void test3()
{
	std::cout << "[independence test] fetch blocks with permutation [generator]" << std::endl;
	independence_test_get_blocks_from_generator<true>();
	std::cout << "[independence test] fetch blocks without permutation [generator]" << std::endl;
	independence_test_get_blocks_from_generator<false>();
	std::cout << "[two-sample test] fetch blocks with permutation [generator]" << std::endl;
	two_sample_test_get_blocks_from_generator<true>();
	std::cout << "[two-sample test] fetch blocks without permutation [generator]" << std::endl;
	two_sample_test_get_blocks_from_generator<false>();
}

int main(int argc, char** argv)
{
	init_shogun_with_defaults();
//	sg_io->set_loglevel(MSG_GCDEBUG);
	test1();
	test2();
	test3();
	exit_shogun();
	return 0;
}
