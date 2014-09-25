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

#include <shogun/lib/config.h>
#include <shogun/features/Features.h>
#include <flash/statistics/internals/Permutators.h>
#include <flash/statistics/internals/PermutationPolicy.h>

using namespace std;
using namespace shogun;
using namespace internal;

using TwoSampleTestReturnType = typename TwoSampleTest::return_type;
using IndependenceTestReturnType = typename IndependenceTest::return_type;

TwoSampleTestPermutation::TwoSampleTestPermutation(const vector<shared_ptr<PermutatorBase>>& _permutators)
	: Permutation(_permutators)
{
}

TwoSampleTestReturnType TwoSampleTestPermutation::get(bool simulate_h0)
{
	TwoSampleTestReturnType ret = get_unshuffled();
	if (simulate_h0)
	{
		permutators[0]->permute(ret);
	}
	return ret;
}

TwoSampleTestReturnType TwoSampleTestPermutation::get_unshuffled()
{
	ASSERT(samples.size() == 2);
	TwoSampleTestReturnType ret = samples[0]->create_merged_copy(samples[1]);
	SG_UNREF(samples[0]);
	SG_UNREF(samples[1]);
	return ret;
}

IndependenceTestPermutation::IndependenceTestPermutation(const vector<shared_ptr<PermutatorBase>>& _permutators)
	: Permutation(_permutators)
{
}

IndependenceTestReturnType IndependenceTestPermutation::get(bool simulate_h0)
{
	IndependenceTestReturnType ret = get_unshuffled();
	if (simulate_h0)
	{
		permutators[0]->permute(ret[0]);
	}
	return ret;
}

IndependenceTestReturnType IndependenceTestPermutation::get_unshuffled()
{
	return samples;
}

