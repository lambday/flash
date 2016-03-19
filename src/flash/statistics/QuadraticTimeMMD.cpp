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
#include <flash/statistics/QuadraticTimeMMD.h>
#include <flash/statistics/internals/DataManager.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

CQuadraticTimeMMD::CQuadraticTimeMMD() : CMMD<CQuadraticTimeMMD>()
{
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

const char* CQuadraticTimeMMD::get_name() const
{
	return "QuadraticTimeMMD";
}
