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

#include <shogun/lib/SGMatrix.h>
#include <shogun/lib/GPUMatrix.h>
#include <shogun/mathematics/eigen3.h>
#include <flash/statistics/internals/MMDUnbiasedFull.h>

using namespace Eigen;

namespace shogun
{

namespace internal
{

template <>
typename MMDUnbiasedFull::return_type MMDUnbiasedFull::operator()(SGMatrix<float64_t> km, index_t n_x)
{
	index_t n_y = km.num_rows - n_x;
	Map<MatrixXd> map(km.matrix, km.num_rows, km.num_cols);

	const Block<Map<MatrixXd>>& b_x = map.block(0, 0, n_x, n_x);
	float64_t term_1 = b_x.sum() - b_x.diagonal().sum();

	const Block<Map<MatrixXd>>& b_y = map.block(n_x, n_x, n_y, n_y);
	float64_t term_2 = b_y.sum() - b_y.diagonal().sum();

	const Block<Map<MatrixXd>>& b_xy = map.block(n_x, 0, n_y, n_x);
	float64_t term_3 = b_xy.sum();

	auto statistic = term_1 / n_x / (n_x - 1) + term_2 / n_y / (n_y - 1) - 2 * term_3 / n_x / n_y;
	return statistic;

}

}

}
