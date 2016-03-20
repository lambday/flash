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
#include <flash/statistics/LinearTimeMMD.h>
#include <flash/statistics/BTestMMD.h>

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
	std::cout << "Quadratic time MMD : Unbiased Full" << std::endl;
	auto statistic = mmd->compute_statistic();
	std::cout << "statistic = " << statistic << std::endl;
	auto variance = mmd->compute_variance();
	std::cout << "variance = " << variance << std::endl;

	mmd->set_statistic_type(S_TYPE::UNBIASED_INCOMPLETE);
	std::cout << "Quadratic time MMD : Unbiased Incomplete" << std::endl;
	statistic = mmd->compute_statistic();
	std::cout << "statistic = " << statistic << std::endl;
	variance = mmd->compute_variance();
	std::cout << "variance = " << variance << std::endl;

	mmd->set_statistic_type(S_TYPE::BIASED_FULL);
	std::cout << "Quadratic time MMD : Biased Full" << std::endl;
	statistic = mmd->compute_statistic();
	std::cout << "statistic = " << statistic << std::endl;
	variance = mmd->compute_variance();
	std::cout << "variance = " << variance << std::endl;
}

void test4(CFeatures* feats_p, CFeatures* feats_q, CKernel* kernel)
{
	auto mmd = std::make_unique<CQuadraticTimeMMD>();
	mmd->set_p(feats_p);
	mmd->set_q(feats_q);
	mmd->set_kernel(kernel);

	mmd->set_statistic_type(S_TYPE::UNBIASED_FULL);
	mmd->set_null_approximation_method(N_METHOD::MMD2_SPECTRUM);
	mmd->set_num_null_samples(10);
	mmd->set_num_eigenvalues(2);

	SGVector<float64_t> null_samples = mmd->sample_null();

	std::cout << "Computed null samples using Spectrum" << std::endl;
	null_samples.display_vector();

	mmd->set_null_approximation_method(N_METHOD::PERMUTATION);
	null_samples = mmd->sample_null();

	std::cout << "Computed null samples using permutation" << std::endl;
	null_samples.display_vector();
}

void test5(CFeatures* feats_p, CFeatures* feats_q, CKernel* kernel)
{
	auto mmd = std::make_unique<CQuadraticTimeMMD>();
	mmd->set_p(feats_p);
	mmd->set_q(feats_q);
	mmd->set_kernel(kernel);

	mmd->set_statistic_type(S_TYPE::UNBIASED_FULL);

	mmd->set_num_null_samples(10);
	mmd->set_num_eigenvalues(2);

	float64_t alpha = 0.5;

	mmd->set_null_approximation_method(N_METHOD::MMD2_SPECTRUM);
	bool result = mmd->perform_test(alpha);
	std::cout << "QuadraticTimeMMD::(Unbiased Full, Spectrum) : result = " << result << std::endl;

	mmd->set_null_approximation_method(N_METHOD::PERMUTATION);
	result = mmd->perform_test(alpha);
	std::cout << "QuadraticTimeMMD::(Unbiased Full, permutation) : result = " << result << std::endl;

	// gamma is only possible with equal number of samples
	index_t n_p = feats_p->get_num_vectors();
	SGVector<index_t> inds(n_p);
	std::iota(inds.vector, inds.vector + inds.vlen, 0);
	feats_q->add_subset(inds);
	auto subset_of_q = static_cast<CFeatures*>(feats_q->clone());
	feats_q->remove_subset();
	feats_q = subset_of_q;

	mmd->set_q(feats_q);

	mmd->set_null_approximation_method(N_METHOD::MMD2_GAMMA);
	mmd->set_statistic_type(S_TYPE::BIASED_FULL);
	result = mmd->perform_test(alpha);
	std::cout << "QuadraticTimeMMD::(Unbiased Full, gamma) : result = " << result << std::endl;
}

void test2(CFeatures* feats_p, CFeatures* feats_q, CKernel* kernel)
{
	auto mmd = std::make_unique<CBTestMMD>();
	mmd->set_p(feats_p);
	mmd->set_q(feats_q);
	mmd->set_kernel(kernel);

	const index_t total_num_feats = feats_p->get_num_vectors() + feats_q->get_num_vectors();
	mmd->set_blocksize(total_num_feats / 5);
	mmd->set_num_blocks_per_burst(3);

	mmd->set_statistic_type(S_TYPE::UNBIASED_FULL);
	std::cout << "B-test : Unbiased Full" << std::endl;
	auto statistic = mmd->compute_statistic();
	std::cout << "statistic = " << statistic << std::endl;
	auto variance = mmd->compute_variance();
	std::cout << "variance = " << variance << std::endl;

	mmd->set_statistic_type(S_TYPE::UNBIASED_INCOMPLETE);
	std::cout << "B-test : Unbiased Incomplete" << std::endl;
	statistic = mmd->compute_statistic();
	std::cout << "statistic = " << statistic << std::endl;
	variance = mmd->compute_variance();
	std::cout << "variance = " << variance << std::endl;

	mmd->set_statistic_type(S_TYPE::BIASED_FULL);
	std::cout << "B-test : Biased Full" << std::endl;
	statistic = mmd->compute_statistic();
	std::cout << "statistic = " << statistic << std::endl;
	variance = mmd->compute_variance();
	std::cout << "variance = " << variance << std::endl;
}

void test6(CFeatures* feats_p, CFeatures* feats_q, CKernel* kernel)
{
	auto mmd = std::make_unique<CBTestMMD>();
	mmd->set_p(feats_p);
	mmd->set_q(feats_q);
	mmd->set_kernel(kernel);

	const index_t total_num_feats = feats_p->get_num_vectors() + feats_q->get_num_vectors();
	mmd->set_blocksize(total_num_feats / 5);
	mmd->set_num_blocks_per_burst(3);

	float64_t alpha = 0.5;

	mmd->set_statistic_type(S_TYPE::UNBIASED_FULL);

	mmd->set_null_approximation_method(N_METHOD::MMD1_GAUSSIAN);
	bool result = mmd->perform_test(alpha);
	std::cout << "BTestMMD::(Unbiased Full, Gaussian) : result = " << result << std::endl;

	mmd->set_null_approximation_method(N_METHOD::PERMUTATION);
	mmd->set_num_null_samples(10);
	result = mmd->perform_test(alpha);
	std::cout << "BTestMMD::(Unbiased Full, permutation) : result = " << result << std::endl;

	mmd->set_statistic_type(S_TYPE::BIASED_FULL);

	mmd->set_null_approximation_method(N_METHOD::MMD1_GAUSSIAN);
	result = mmd->perform_test(alpha);
	std::cout << "BTestMMD::(Biased Full, Gaussian) : result = " << result << std::endl;

	mmd->set_null_approximation_method(N_METHOD::PERMUTATION);
	mmd->set_num_null_samples(10);
	result = mmd->perform_test(alpha);
	std::cout << "BTestMMD::(Biased Full, permutation) : result = " << result << std::endl;
}

void test3(CFeatures* feats_p, CFeatures* feats_q, CKernel* kernel)
{
	auto mmd = std::make_unique<CLinearTimeMMD>();
	mmd->set_p(feats_p);
	mmd->set_q(feats_q);
	mmd->set_kernel(kernel);

	mmd->set_num_blocks_per_burst(5);

	mmd->set_statistic_type(S_TYPE::UNBIASED_FULL);
	std::cout << "Linear time MMD : Unbiased Full" << std::endl;
	auto statistic = mmd->compute_statistic();
	std::cout << "statistic = " << statistic << std::endl;
	auto variance = mmd->compute_variance();
	std::cout << "variance = " << variance << std::endl;

	mmd->set_statistic_type(S_TYPE::UNBIASED_INCOMPLETE);
	std::cout << "Linear time MMD : Unbiased Incomplete" << std::endl;
	statistic = mmd->compute_statistic();
	std::cout << "statistic = " << statistic << std::endl;
	variance = mmd->compute_variance();
	std::cout << "variance = " << variance << std::endl;

	mmd->set_statistic_type(S_TYPE::BIASED_FULL);
	std::cout << "Linear time MMD : Biased Full" << std::endl;
	statistic = mmd->compute_statistic();
	std::cout << "statistic = " << statistic << std::endl;
	variance = mmd->compute_variance();
	std::cout << "variance = " << variance << std::endl;
}

void test7(CFeatures* feats_p, CFeatures* feats_q, CKernel* kernel)
{
	auto mmd = std::make_unique<CLinearTimeMMD>();
	mmd->set_p(feats_p);
	mmd->set_q(feats_q);
	mmd->set_kernel(kernel);

	mmd->set_num_blocks_per_burst(5);

	float64_t alpha = 0.5;

	mmd->set_statistic_type(S_TYPE::UNBIASED_FULL);

	mmd->set_null_approximation_method(N_METHOD::MMD1_GAUSSIAN);
	bool result = mmd->perform_test(alpha);
	std::cout << "LinearTimeMMD::(Unbiased Full, Gaussian) : result = " << result << std::endl;

	mmd->set_null_approximation_method(N_METHOD::PERMUTATION);
	mmd->set_num_null_samples(10);
	result = mmd->perform_test(alpha);
	std::cout << "LinearTimeMMD::(Unbiased Full, permutation) : result = " << result << std::endl;

	mmd->set_statistic_type(S_TYPE::BIASED_FULL);

	mmd->set_null_approximation_method(N_METHOD::MMD1_GAUSSIAN);
	result = mmd->perform_test(alpha);
	std::cout << "LinearTimeMMD::(Biased Full, Gaussian) : result = " << result << std::endl;

	mmd->set_null_approximation_method(N_METHOD::PERMUTATION);
	mmd->set_num_null_samples(10);
	result = mmd->perform_test(alpha);
	std::cout << "LinearTimeMMD::(Biased Full, permutation) : result = " << result << std::endl;
}

int main()
{
	init_shogun_with_defaults();

	const index_t dim = 2;
	const index_t num_vec_p = 20;
	const index_t num_vec_q = 30;

	SGMatrix<float64_t> data_p(dim, num_vec_p);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec_p, 2);
	std::for_each(data_p.matrix, data_p.matrix + dim * num_vec_p, [](auto& v) { v = sin(v); });
	data_p.display_matrix();

	SGMatrix<float64_t> data_q(dim, num_vec_q);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec_q, 2);
	std::for_each(data_q.matrix, data_q.matrix + dim * num_vec_q, [](auto& v) { v = log(v); });
	data_q.display_matrix();

	auto feats_p = new CDenseFeatures<float64_t>(data_p);
	auto feats_q = new CDenseFeatures<float64_t>(data_q);

	SG_REF(feats_p);
	SG_REF(feats_q);

	auto kernel = new CGaussianKernel();
	SG_REF(kernel);
	kernel->set_width(0.5);

	test1(feats_p, feats_q, kernel);
	test2(feats_p, feats_q, kernel);
	test3(feats_p, feats_q, kernel);

	test4(feats_p, feats_q, kernel);
	test5(feats_p, feats_q, kernel);
	test6(feats_p, feats_q, kernel);
	test7(feats_p, feats_q, kernel);

	SG_UNREF(feats_p);
	SG_UNREF(feats_q);
	SG_UNREF(kernel);

	exit_shogun();
	return 0;
}
