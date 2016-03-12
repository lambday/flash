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

#include <iostream> // TODO remove
#include <flash/statistics/internals/DataManager.h>
#include <flash/statistics/internals/NextSamples.h>
#include <flash/statistics/internals/DataFetcher.h>
#include <flash/statistics/internals/DataFetcherFactory.h>
#include <shogun/features/Features.h>

using namespace shogun;
using namespace internal;

DataManager::DataManager(index_t num_distributions)
{
	fetchers.resize(num_distributions);
	std::fill(fetchers.begin(), fetchers.end(), nullptr);
}

DataManager::~DataManager()
{
}

index_t DataManager::get_num_samples()
{
	int n = 0;
	if (std::any_of(fetchers.begin(), fetchers.end(), [](auto& f) { return f->m_num_samples == 0; }))
	{
		std::cout << "number of samples from all the distributions are not set" << std::endl;
	}
	else
	{
		std::for_each(fetchers.begin(), fetchers.end(), [&n](auto &f) { n+= f->m_num_samples; });
	}
	return n;
}

void DataManager::set_blocksize(index_t blocksize)
{
	auto n = get_num_samples();

	ASSERT(n > 0);
	ASSERT(blocksize > 0 && blocksize <= n);
	ASSERT(n % blocksize == 0);

	for (auto i = 0; i < fetchers.size(); ++i)
	{
		index_t m = fetchers[i]->m_num_samples;
		ASSERT((blocksize * m) % n == 0);
		fetchers[i]->fetch_blockwise().with_blocksize(blocksize * m / n);
		std::cout << "block[" << i << "].size = " << blocksize * m / n << std::endl;
	}
}

void DataManager::set_num_blocks_per_burst(index_t num_blocks_per_burst)
{
	ASSERT(num_blocks_per_burst > 0);

	index_t blocksize = 0;
	std::for_each(fetchers.begin(), fetchers.end(), [&blocksize](auto& f) { blocksize += f->m_block_details.m_blocksize; });
	ASSERT(blocksize > 0);

	index_t max_num_blocks_per_burst = get_num_samples() / blocksize;
	ASSERT(num_blocks_per_burst <= max_num_blocks_per_burst);

	for (auto i = 0; i < fetchers.size(); ++i)
	{
		fetchers[i]->fetch_blockwise().with_num_blocks_per_burst(num_blocks_per_burst);
	}
}

InitPerFeature DataManager::samples_at(index_t i)
{
	std::cout << "DataManager::samples_at()" << std::endl;
	ASSERT(i < fetchers.size());
	return InitPerFeature(fetchers[i]);
}

const CFeatures* DataManager::samples_at(index_t i) const
{
	std::cout << "DataManager::samples_at() const" << std::endl;
	ASSERT(i < fetchers.size());
	return fetchers[i]->m_samples.get();
}

index_t& DataManager::num_samples_at(index_t i)
{
	std::cout << "DataManager::num_samples_at()" << std::endl;
	ASSERT(i < fetchers.size());
	return fetchers[i]->m_num_samples;
}

const index_t DataManager::num_samples_at(index_t i) const
{
	std::cout << "DataManager::num_samples_at() const" << std::endl;
	ASSERT(i < fetchers.size());
	return fetchers[i]->m_num_samples;
}

void DataManager::start()
{
	std::for_each(fetchers.begin(), fetchers.end(), [](auto& f) { f->start(); });
}

std::shared_ptr<NextSamples> DataManager::next()
{
	std::cout << "DataManager::next()" << std::endl;
	auto next_samples = std::make_shared<NextSamples>(fetchers.size());
	for (auto i = 0; i < fetchers.size(); ++i)
	{
		next_samples->at(i) = fetchers[i]->next();
	}
	return next_samples;
}

void DataManager::end()
{
	std::for_each(fetchers.begin(), fetchers.end(), [](auto& f) { f->end(); });
}

void DataManager::reset()
{
	std::for_each(fetchers.begin(), fetchers.end(), [](auto& f) { f->reset(); });
}
