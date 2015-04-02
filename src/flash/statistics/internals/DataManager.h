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
	// preallocate memoty for the samples, num_samples fetchers and permutators.
    // This is never goind to change for a test type
	DataManager() : simulate_h0(false), _blocksize(0)
	{
		samples.resize(TestType::num_feats);
		num_samples.resize(TestType::num_feats);
		fetchers.resize(TestType::num_feats);
		permutators.resize(TestType::num_feats);

		std::fill(samples.begin(), samples.end(), nullptr);
		std::fill(num_samples.begin(), num_samples.end(), 0);
		std::fill(fetchers.begin(), fetchers.end(), nullptr);
		std::fill(permutators.begin(), permutators.end(), nullptr);
	}
	~DataManager()
	{
		for (auto it = samples.begin(); it != samples.end(); ++it)
		{
			SG_UNREF(*it);
		}
	}

	void set_simulate_h0(bool is_simulate_h0)
	{
		simulate_h0 = is_simulate_h0;
	}

	bool get_simulate_h0()
	{
		return simulate_h0;
	}

	// make it index specific so that the order of the call doesn't matter
	// @param n number of samples
	// @param i the index for which the number of samples is n
	void set_num_samples(index_t n, index_t i)
	{
		ASSERT(i < num_samples.size());
		num_samples[i] = n;
	}

	index_t get_num_samples()
	{
		// TODO null check for empty samples

		// if samples are set, then we don't need to do anything.
		if (std::any_of(num_samples.begin(), num_samples.end(), [](index_t val) { return val == 0; }))
		{
			for (index_t i = 0; i < samples.size(); ++i)
			{
				// this is useful when a streaming feature is used in accordance with
				// a dense feature. we can just set num_samples for the streaming
				// features and keep the other one as zero. then this method will
				// lazily assign number of samples for the dense feature.
				if (num_samples[i] == 0)
					num_samples[i] = samples[i]->get_num_vectors();
			}
		}

		return std::accumulate(num_samples.begin(), num_samples.end(), 0);
	}

	// blocksize is the total number of samples (num_vec_p + num_vec_q)
	void set_blocksize(index_t blocksize)
	{
		index_t n = get_num_samples();

		// TODO replace with REQUIRE
		ASSERT(n > 0);
		ASSERT(blocksize > 0 && blocksize <= n);
		ASSERT(n % blocksize == 0);
		// ASSERT(num_samples.size() == fetchers.size()); // not required anymore

		for (index_t i = 0; i < num_samples.size(); ++i)
		{
			index_t m = num_samples[i];
			ASSERT((blocksize * m) % n == 0);
			fetchers[i]->set_blocksize(blocksize * m / n);
			// TODO set logger for the logs
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

	// put feats in index i
	void put(CFeatures* feats, index_t i)
	{
		std::cout << "DataManager::put" << std::endl;

		ASSERT(i < samples.size());
		samples[i] = feats;
		SG_REF(feats);
		fetchers[i] = FetcherFactory::get_instance(feats);
		permutators[i] = PermutatorFactory::get_instance(feats);
	}

	typename TestType::return_type get_samples()
	{
		std::cout << "DataManager::get_samples" << std::endl;
		// what permutation this will be, that depends on the test type
		// this stores permutator(s) inside it and calls permute method
		// to perform the permutation. but whether to merge samples or not
		// whether to permute samples from one distribution or both, that
		// logic does inside this permutation. Just one instance per get_sample
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
	// multiple fetchers option is there for the same reason as permutators
	std::vector<std::shared_ptr<FetcherBase>> fetchers;
	// multiple permutators are useful when we want to pass two
	// different features types as p and q
	std::vector<std::shared_ptr<PermutatorBase>> permutators;
};

}

}

#endif // DATA_MANAGER_H__
