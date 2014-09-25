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

#ifndef PERMUTAITON_POLICY_H_
#define PERMUTAITON_POLICY_H_

#include <vector>
#include <memory>
#include <shogun/lib/config.h>
#include <flash/statistics/internals/Permutators.h>
#include <flash/statistics/internals/TestTypes.h>

namespace shogun
{

class CFeatures;
template <class T> class CDenseFeatures;

namespace internal
{

template <class T> struct Permutators;

template <class TestType>
struct Permutation
{
	using return_type = typename TestType::return_type;

	Permutation(const std::vector<std::shared_ptr<PermutatorBase>>& _permutators)
		: permutators(_permutators)
	{
	}
	inline void push_back(CFeatures* feats)
	{
		samples.push_back(feats);
	}
	virtual return_type get(bool simulate_h0) = 0;
	std::vector<CFeatures*> samples;
	const std::vector<std::shared_ptr<PermutatorBase>>& permutators;
};

class TwoSampleTestPermutation : public Permutation<TwoSampleTest>
{
public:
	using return_type = typename TwoSampleTest::return_type;
	TwoSampleTestPermutation(const std::vector<std::shared_ptr<PermutatorBase>>& _permutators);
	virtual return_type get(bool simulate_h0) override;
private:
	return_type get_unshuffled();
};

class IndependenceTestPermutation : public Permutation<IndependenceTest>
{
public:
	using return_type = typename IndependenceTest::return_type;
	IndependenceTestPermutation(const std::vector<std::shared_ptr<PermutatorBase>>& _permutators);
	virtual return_type get(bool simulate_h0) override;
private:
	return_type get_unshuffled();
};

}

}

#endif // PERMUTAITON_POLICY_H_
