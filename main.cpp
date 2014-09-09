#include <test_data.h>
#include <shogun/base/init.h>
#include <shogun/lib/SGMatrix.h>
#include <algorithm>
#include <iostream>

using namespace shogun;
using namespace std;

void test1()
{
	std::cout << "independence test data : without permutation" << std::endl;
	const index_t num_feats = 3;
	const index_t num_vec = 4;
	SGMatrix<float64_t> data_p(num_feats, num_vec);
	SGMatrix<float64_t> data_q(num_feats, num_vec);
	std::iota(data_p.matrix, data_p.matrix + num_feats * num_vec, 1);
	std::iota(data_q.matrix, data_q.matrix + num_feats * num_vec, data_p.matrix[num_feats*num_vec-1]+1);

	using feats_type = CDenseFeatures<float64_t>;
	feats_type* feats_p = new feats_type(data_p);
	feats_type* feats_q = new feats_type(data_q);

	TestDataManager<feats_type, FetchAll<feats_type>, IndependenceTestPermutation<feats_type> > data_manager;
	data_manager.samples.push_back(feats_p);
	data_manager.samples.push_back(feats_q);

	std::vector<feats_type*> samples = data_manager.get_samples<false>(FetchAll<feats_type>());
	for (auto sample : samples)
		sample->get_feature_matrix().display_matrix();

	SG_UNREF(feats_p);
	SG_UNREF(feats_q);
}

void test2()
{
	std::cout << "independence test data : with permutation" << std::endl;
	const index_t num_feats = 3;
	const index_t num_vec = 4;
	SGMatrix<float64_t> data_p(num_feats, num_vec);
	SGMatrix<float64_t> data_q(num_feats, num_vec);
	std::iota(data_p.matrix, data_p.matrix + num_feats * num_vec, 1);
	std::iota(data_q.matrix, data_q.matrix + num_feats * num_vec, data_p.matrix[num_feats*num_vec-1]+1);

	using feats_type = CDenseFeatures<float64_t>;
	feats_type* feats_p = new feats_type(data_p);
	feats_type* feats_q = new feats_type(data_q);

	TestDataManager<feats_type, FetchAll<feats_type>, IndependenceTestPermutation<feats_type> > data_manager;
	data_manager.samples.push_back(feats_p);
	data_manager.samples.push_back(feats_q);

	std::vector<feats_type*> samples = data_manager.get_samples<true>(FetchAll<feats_type>());
	for (auto sample : samples)
		sample->get_feature_matrix().display_matrix();

	SG_UNREF(feats_p);
	SG_UNREF(feats_q);
}

void test3()
{
	std::cout << "two-sample test data : without permutation" << std::endl;
	const index_t num_feats = 3;
	const index_t num_vec = 4;
	SGMatrix<float64_t> data_p(num_feats, num_vec);
	SGMatrix<float64_t> data_q(num_feats, num_vec);
	std::iota(data_p.matrix, data_p.matrix + num_feats * num_vec, 1);
	std::iota(data_q.matrix, data_q.matrix + num_feats * num_vec, data_p.matrix[num_feats*num_vec-1]+1);

	using feats_type = CDenseFeatures<float64_t>;
	feats_type* feats_p = new feats_type(data_p);
	feats_type* feats_q = new feats_type(data_q);

	TestDataManager<feats_type, FetchAll<feats_type>, TwoSampleTestPermutation<feats_type> > data_manager;
	data_manager.samples.push_back(feats_p);
	data_manager.samples.push_back(feats_q);

	feats_type* samples = data_manager.get_samples<false>(FetchAll<feats_type>());
	samples->get_feature_matrix().display_matrix();

	SG_UNREF(feats_p);
	SG_UNREF(feats_q);
}

void test4()
{
	std::cout << "two-sample test data : with permutation" << std::endl;
	const index_t num_feats = 3;
	const index_t num_vec = 4;
	SGMatrix<float64_t> data_p(num_feats, num_vec);
	SGMatrix<float64_t> data_q(num_feats, num_vec);
	std::iota(data_p.matrix, data_p.matrix + num_feats * num_vec, 1);
	std::iota(data_q.matrix, data_q.matrix + num_feats * num_vec, data_p.matrix[num_feats*num_vec-1]+1);

	using feats_type = CDenseFeatures<float64_t>;
	feats_type* feats_p = new feats_type(data_p);
	feats_type* feats_q = new feats_type(data_q);

	TestDataManager<feats_type, FetchAll<feats_type>, TwoSampleTestPermutation<feats_type> > data_manager;
	data_manager.samples.push_back(feats_p);
	data_manager.samples.push_back(feats_q);

	feats_type* samples = data_manager.get_samples<true>(FetchAll<feats_type>());
	samples->get_feature_matrix().display_matrix();

	SG_UNREF(feats_p);
	SG_UNREF(feats_q);
}
int main(int argc, char** argv)
{
	init_shogun_with_defaults();
	test1();
	test2();
	test3();
	test4();
	exit_shogun();
	return 0;
}
