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

#include <iostream> // TODO remove
#include <flash/statistics/internals/DataManager.h>
#include <flash/statistics/internals/DataFetcher.h>
#include <flash/statistics/internals/DataFetcherFactory.h>
#include <shogun/features/Features.h>

using namespace shogun;
using namespace internal;

InitPerFeature::InitPerFeature(DataManager& dm, index_t i) : data_manager(dm), index(i)
{
	std::cout << "InitPerFeature::Constructor() with i = " << i << std::endl;
	ASSERT(i < data_manager.fetchers.size());
}

InitPerFeature::~InitPerFeature()
{
	std::cout << "InitPerFeature::Destructor()" << std::endl;
}

InitPerFeature& InitPerFeature::operator=(CFeatures* feats)
{
	std::cout << "InitPerFeature::Assignment() : setting the fetcher" << std::endl;
	std::unique_ptr<DataFetcher> fetcher(DataFetcherFactory::get_instance(feats));
	data_manager.fetchers[index] = std::move(fetcher);
	return *this;
}
