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

#include <flash/statistics/LinearTimeMMD.h>
#include <flash/statistics/internals/DataManager.h>
#include <shogun/mathematics/Math.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

CLinearTimeMMD::CLinearTimeMMD() : CMMD<CLinearTimeMMD>()
{
}

CLinearTimeMMD::~CLinearTimeMMD()
{
}

void CLinearTimeMMD::set_num_blocks_per_burst(index_t num_blocks_per_burst)
{
	get_data_manager().set_blocksize(get_data_manager().get_min_blocksize());
	get_data_manager().set_num_blocks_per_burst(num_blocks_per_burst);
}

internal::mmd::WithinBlockDirect CLinearTimeMMD::get_direct_estimation_method()
{
	static internal::mmd::WithinBlockDirect method;
	return method;
}

const float64_t CLinearTimeMMD::normalize_statistic(float64_t statistic) const
{
	const DataManager& dm = get_data_manager();
	const index_t Nx = dm.num_samples_at(0);
	const index_t Ny = dm.num_samples_at(1);
	return CMath::sqrt(Nx * Ny / float64_t(Nx + Ny)) * statistic;
}

const float64_t CLinearTimeMMD::normalize_variance(float64_t variance) const
{
	const DataManager& dm = get_data_manager();
	const index_t Bx = dm.blocksize_at(0);
	const index_t By = dm.blocksize_at(1);
	const index_t B = Bx + By;
	return variance * Bx * By * (Bx - 1) * (By - 1) / (B - 1) / (B - 2);
}

const char* CLinearTimeMMD::get_name() const
{
	return "LinearTimeMMD";
}
