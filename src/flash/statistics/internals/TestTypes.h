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

#ifndef TEST_TYPES_H__
#define TEST_TYPES_H__

#include <vector>
#include <shogun/lib/config.h>

namespace shogun
{

class CFeatures;

namespace internal
{

struct OneDistributionTest
{
	static constexpr index_t num_feats = 1;
};

struct TwoDistributionTest
{
	static constexpr index_t num_feats = 2;
};

struct ThreeDistributionTest
{
	static constexpr index_t num_feats = 3;
};

struct GoodnessOfFitTest : OneDistributionTest
{
	static constexpr index_t num_kernels = 1;
};

struct TwoSampleTest : TwoDistributionTest
{
	static constexpr index_t num_kernels = 1;
};

struct IndependenceTest : TwoDistributionTest
{
	static constexpr index_t num_kernels = 2;
};

}

}

#endif // TEST_TYPES_H__
