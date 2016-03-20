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

#include <shogun/io/SGIO.h>
#include <shogun/lib/SGVector.h>
#include <shogun/kernel/Kernel.h>
#include <shogun/kernel/CustomKernel.h>
#include <shogun/mathematics/eigen3.h>
#include <flash/statistics/QuadraticTimeMMD.h>
#include <flash/statistics/internals/NextSamples.h>
#include <flash/statistics/internals/DataManager.h>
#include <flash/statistics/internals/KernelManager.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

struct CQuadraticTimeMMD::Self
{
	Self();
	index_t num_eigenvalues;
};

CQuadraticTimeMMD::Self::Self() : num_eigenvalues(0)
{
}

CQuadraticTimeMMD::CQuadraticTimeMMD() : CMMD<CQuadraticTimeMMD>()
{
	self = std::make_unique<Self>();
}

CQuadraticTimeMMD::~CQuadraticTimeMMD()
{
}

internal::mmd::FullDirect CQuadraticTimeMMD::get_direct_estimation_method()
{
	static internal::mmd::FullDirect method;
	return method;
}

const float64_t CQuadraticTimeMMD::normalize_statistic(float64_t statistic) const
{
	const DataManager& dm = get_data_manager();
	const index_t Nx = dm.num_samples_at(0);
	const index_t Ny = dm.num_samples_at(1);
	return Nx * Ny * statistic / (Nx + Ny);
}

const float64_t CQuadraticTimeMMD::normalize_variance(float64_t variance) const
{
	SG_SNOTIMPLEMENTED;
	return variance;
}

void CQuadraticTimeMMD::set_num_eigenvalues(index_t num_eigenvalues)
{
	self->num_eigenvalues = num_eigenvalues;
}

float64_t CQuadraticTimeMMD::compute_p_value(float64_t statistic)
{
	float64_t result = 0;
	switch (get_null_approximation_method())
	{
		case N_METHOD::MMD2_GAMMA:
		{
			// TODO
		}
		break;
		default:
			result = CHypothesisTest::compute_p_value(statistic);
		break;
	}
	return result;
}

float64_t CQuadraticTimeMMD::compute_threshold(float64_t alpha)
{
	float64_t result = 0;
	switch (get_null_approximation_method())
	{
		case N_METHOD::MMD2_GAMMA:
		{
			// TODO
		}
		break;
		default:
			result = CHypothesisTest::compute_threshold(alpha);
		break;
	}
	return result;
}

SGVector<float64_t> CQuadraticTimeMMD::sample_null()
{
	if (get_null_approximation_method() == N_METHOD::MMD2_SPECTRUM)
	{
		DataManager& dm = get_data_manager();
		index_t m = dm.num_samples_at(0);
		index_t n = dm.num_samples_at(1);

		if (self->num_eigenvalues > m + n - 1)
		{
			SG_ERROR("Number of Eigenvalues too large\n");
		}

		if (self->num_eigenvalues < 1)
		{
			SG_ERROR("Number of Eigenvalues too small\n");
		}

		dm.start();
		auto next_samples = dm.next();

		SGVector<float64_t> null_samples(get_num_null_samples());
		std::fill(null_samples.vector, null_samples.vector + null_samples.vlen, 0);

		if (!next_samples.empty())
		{
			auto feats_p = next_samples[0][0];
			auto feats_q = next_samples[1][0];

			auto feats_p_q = feats_p->create_merged_copy(feats_q.get());

			CKernel *kernel = get_kernel_manager().kernel_at(0);
			kernel->init(feats_p_q, feats_p_q);
			auto precomputed = std::make_unique<CCustomKernel>(kernel);
			kernel->remove_lhs_and_rhs();

			/* imaginary matrix K=[K KL; KL' L] (MATLAB notation)
			 * K is matrix for XX, L is matrix for YY, KL is XY, LK is YX
			 * works since X and Y are concatenated here */
			SGMatrix<float64_t> K = precomputed->get_kernel_matrix();

			/* center matrix K=H*K*H */
			K.center();

			/* compute eigenvalues and select num_eigenvalues largest ones */
			Eigen::Map<Eigen::MatrixXd> c_kernel_matrix(K.matrix, K.num_rows, K.num_cols);
			Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigen_solver(c_kernel_matrix);
			REQUIRE(eigen_solver.info() == Eigen::Success, "Eigendecomposition failed!\n");
			index_t max_num_eigenvalues = eigen_solver.eigenvalues().rows();

			/* finally, sample from null distribution */

#pragma omp parallel for
			for (auto i = 0; i < null_samples.vlen; ++i)
			{
				float64_t null_sample = 0;
				for (index_t j = 0; j < self->num_eigenvalues; ++j)
				{
					float64_t z_j = CMath::randn_double();
					float64_t multiple = CMath::sq(z_j);

					/* take largest EV, scale by 1/(m+n) on the fly and take abs value*/
					float64_t eigenvalue_estimate = eigen_solver.eigenvalues()[max_num_eigenvalues-1-j];
					eigenvalue_estimate /= (m + n);

					if (get_statistic_type() == S_TYPE::UNBIASED_FULL)
					{
						multiple -= 1;
					}

					null_sample += eigenvalue_estimate * multiple;
				}
				null_samples[i] = null_sample;
			}
		}

		return null_samples;
	}
	else
	{
		return CMMD::sample_null();
	}
}

const char* CQuadraticTimeMMD::get_name() const
{
	return "QuadraticTimeMMD";
}
