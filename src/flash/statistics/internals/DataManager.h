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

#ifndef DATA_MANAGER_H__
#define DATA_MANAGER_H__

#include <vector>
#include <memory>
#include <flash/statistics/internals/InitPerFeature.h>
#include <shogun/lib/common.h>

namespace shogun
{

class CFeatures;

namespace internal
{

class DataFetcher;
class NextSamples;

class DataManager
{
public:
	DataManager(index_t num_distributions);
	DataManager(const DataManager& other) = delete;
	DataManager& operator=(const DataManager& other) = delete;
	~DataManager();
	void set_blocksize(index_t blocksize);
	void set_num_blocks_per_burst(index_t num_blocks_per_burst);
	// make it only available for streaming
	// something like data_manager.streaming().num_samples_at(0) = 10
	index_t& num_samples_at(index_t i);
	index_t get_num_samples();
	// if we provide an opeartor= in init task per feature that
	// accepts a file, the samples_at(i) = someFileObject will also work!
	InitPerFeature samples_at(index_t i);
	void start();
	std::shared_ptr<NextSamples> next();
	void end();
	void reset();
private:
	const index_t m_num_distributions;
	// multiple fetchers option is there for heterogenous features
	std::vector<std::unique_ptr<DataFetcher>> fetchers;
};

}

}

#endif // DATA_MANAGER_H__
