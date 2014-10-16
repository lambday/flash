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
#include <numeric>
#include <iostream> // TODO remove
#include <shogun/lib/config.h>
#include <flash/statistics/internals/Permutators.h>
#include <flash/statistics/internals/FetcherPolicy.h>
#include <flash/statistics/internals/PermutationPolicy.h>
#include <flash/statistics/internals/FetcherFactory.h>
#include <flash/statistics/internals/PermutatorFactory.h>
#include <shogun/features/Features.h> // TODO remove - move the implementation to cpp

namespace shogun
{

class Features;

namespace internal
{

template <class TestType>
class DataManager
{
public:
	DataManager() : simulate_h0(false), _blocksize(0)
	{
		samples.reserve(2);
		samples.reserve(2);
		fetchers.reserve(2);
		permutators.reserve(2);
	}

	void set_simulate_h0(bool is_simulate_h0)
	{
		simulate_h0 = is_simulate_h0;
	}

	bool get_simulate_h0()
	{
		return simulate_h0;
	}

	void set_num_samples(index_t n)
	{
		num_samples.push_back(n);
	}

	index_t get_num_samples()
	{
		if (num_samples.size() == 0)
		{
			for (auto feats : samples)
				num_samples.push_back(feats->get_num_vectors());
		}

		return std::accumulate(num_samples.begin(), num_samples.end(), 0);
	}

	void set_blocksize(index_t blocksize)
	{
		index_t n = get_num_samples();

		ASSERT(n > 0);
		ASSERT(blocksize > 0 && blocksize <= n);
		ASSERT(n % blocksize == 0);
		ASSERT(num_samples.size() == fetchers.size());

		for (index_t i = 0; i < num_samples.size(); ++i)
		{
			index_t m = num_samples[i];
			ASSERT((blocksize * m) % n == 0);
			fetchers[i]->set_blocksize(blocksize * m / n);
			std::cout << "block[" << i << "].size = " << blocksize * m / n << std::endl;
		}
		_blocksize = blocksize;
	}

	void set_num_blocks_per_burst(index_t num_blocks_per_burst)
	{
		ASSERT(_blocksize > 0);
		ASSERT(num_blocks_per_burst > 0);

		index_t max_num_blocks_per_burst = get_num_samples()/_blocksize;
		ASSERT(num_blocks_per_burst <= max_num_blocks_per_burst);

		for (auto fetcher : fetchers)
		{
			fetcher->set_num_blocks_per_burst(num_blocks_per_burst);
		}
	}

	void push_back(CFeatures* feats)
	{
		std::cout << "DataManager::push_back" << std::endl;

		samples.push_back(feats);
		fetchers.push_back(FetcherFactory::get_instance(feats));
		permutators.push_back(PermutatorFactory::get_instance(feats));
	}

	typename TestType::return_type get_samples()
	{
		std::cout << "DataManager::get_samples" << std::endl;
		std::shared_ptr<typename TestType::permutation_type> permutation(new
				typename TestType::permutation_type(permutators));
		for (size_t i = 0; i < samples.size(); ++i)
		{
			permutation->push_back(fetchers[i]->fetch(samples[i]));
		}
		return permutation->get(simulate_h0);
	}
private:
	bool simulate_h0;
	index_t _blocksize;
	std::vector<shogun::CFeatures*> samples;
	std::vector<index_t> num_samples;
	std::vector<std::shared_ptr<FetcherBase>> fetchers;
	std::vector<std::shared_ptr<PermutatorBase>> permutators;
};

}

}

#endif // DATA_MANAGER_H__
