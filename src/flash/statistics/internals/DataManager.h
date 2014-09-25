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

#ifndef DATA_MANAGER_H__
#define DATA_MANAGER_H__

#include <vector>
#include <memory>
#include <shogun/lib/config.h>
#include <flash/statistics/internals/Permutators.h>
#include <flash/statistics/internals/FetcherPolicy.h>
#include <flash/statistics/internals/PermutationPolicy.h>

namespace shogun
{

class Features;

namespace internal
{

template <class TestType>
class DataManager
{
public:
	void set_simulate_h0(bool is_simulate_h0)
	{
		simulate_h0 = is_simulate_h0;
	}
	bool get_simulate_h0()
	{
		return simulate_h0;
	}

	template <class F>
	void push_back(F* feats)
	{
		static_assert(std::is_base_of<CFeatures, F>::value, "Unsupported feature type provided!\n");
		static_assert(!std::is_same<CFeatures, F>::value, "Please provide feature with actual type!\n");

		samples.push_back(feats);

		fetchers.push_back(TestType::template fetch_type<F>::get_instance());
		permutators.push_back(Permutator<typename fetch_traits<F>::return_type>::get_instance());
	}

	typename TestType::return_type get_samples()
	{
		std::shared_ptr<Permutation<TestType>> permutation(new typename TestType::permutation_type(permutators));
		for (size_t i = 0; i < samples.size(); ++i)
		{
			permutation->push_back(fetchers[i]->fetch(samples[i]));
		}
		return permutation->get(simulate_h0);
	}
private:
	bool simulate_h0;
	std::vector<shogun::CFeatures*> samples;
	std::vector<std::shared_ptr<FetcherBase>> fetchers;
	std::vector<std::shared_ptr<PermutatorBase>> permutators;
};

}

}

#endif // DATA_MANAGER_H__
