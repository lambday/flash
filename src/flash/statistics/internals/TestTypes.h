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

#ifndef TEST_TYPES_H__
#define TEST_TYPES_H__

#include <vector>
#include <shogun/lib/config.h>

namespace shogun
{

class CFeatures;

namespace internal
{

template <class T> struct AllFetcher;
template <class T> struct BlockFetcher;
struct TwoSampleTestPermutation;
struct IndependenceTestPermutation;

struct BaseTestType
{
	enum { num_feats = 2 };
};

struct TwoSampleTest : public BaseTestType
{
	template <class T> using fetch_type = AllFetcher<T>;
	using permutation_type = TwoSampleTestPermutation;
	using return_type = shogun::CFeatures*;
};

struct IndependenceTest : public BaseTestType
{
	template <class T> using fetch_type = AllFetcher<T>;
	using permutation_type = IndependenceTestPermutation;
	using return_type = std::vector<shogun::CFeatures*>;
};

struct StreamingTwoSampleTest : public BaseTestType
{
	template <class T> using fetch_type = BlockFetcher<T>;
	using permutation_type = TwoSampleTestPermutation;
	using return_type = shogun::CFeatures*;
};

struct StreamingIndependenceTest : public BaseTestType
{
	template <class T> using fetch_type = BlockFetcher<T>;
	using permutation_type = IndependenceTestPermutation;
	using return_type = std::vector<shogun::CFeatures*>;
};

}

}

#endif // TEST_TYPES_H__
