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

#ifndef MMD_UNBIASED_INCOMPLETE_H_
#define MMD_UNBIASED_INCOMPLETE_H_

#include <shogun/lib/common.h>

namespace shogun
{

namespace internal
{

struct MMDUnbiasedIncomplete
{
	using return_type = float64_t;

	template <typename MatrixType>
	return_type operator()(MatrixType kernel_matrix, index_t n_x);
};

}

}

#endif // MMD_UNBIASED_INCOMPLETE_H_
