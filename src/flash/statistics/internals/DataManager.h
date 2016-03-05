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
#include <flash/statistics/internals/InitPerSamples.h>
#include <shogun/lib/common.h>

namespace shogun
{

class CFeatures;

namespace internal
{

class FetcherBase;
class PermutatorBase;

template <class TestType>
class DataManager
{
	template <class T> friend class InitPerSamples;
public:
	// preallocate memory for the samples, num_samples fetchers and permutators.
    // This is never goind to change for a test type
	DataManager();
	~DataManager();

	void set_simulate_h0(bool is_simulate_h0);
	bool get_simulate_h0();

	// @param i the index for which the number of samples is n
	index_t& num_samples_at(index_t i);
	index_t get_num_samples();

	// blocksize is the total number of samples (num_vec_p + num_vec_q)
	void set_blocksize(index_t blocksize);
	void set_num_blocks_per_burst(index_t num_blocks_per_burst);

	// put feats in index i
	InitPerSamples<TestType> samples_at(index_t i);
	typename TestType::return_type get_samples();

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
