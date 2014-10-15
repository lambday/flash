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
#include <shogun/lib/config.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
#include <flash/statistics/internals/FetcherPolicy.h>
#include <flash/statistics/internals/PermutatorFactory.h>
#include <flash/statistics/internals/Permutators.h>

using namespace shogun;
using namespace internal;

const PermutatorFactory::factory& PermutatorFactory::initializers = PermutatorFactory::initialize();

PermutatorFactory::factory PermutatorFactory::initialize()
{
	PermutatorFactory::factory initializers;

	initializers.insert(std::make_pair(std::make_pair(C_DENSE, F_DREAL),
			[](){ return Permutator<fetch_traits<CDenseFeatures<float64_t>>::return_type>::get_instance(); }));
	initializers.insert(std::make_pair(std::make_pair(C_STREAMING_DENSE, F_DREAL),
			[](){ return Permutator<fetch_traits<CDenseFeatures<float64_t>>::return_type>::get_instance(); }));

	return initializers;
}

PermutatorFactory::return_type PermutatorFactory::get_instance(CFeatures* feats)
{
	EFeatureClass feature_class = feats->get_feature_class();
	EFeatureType feature_type = feats->get_feature_type();

	auto it = initializers.find(std::make_pair(feature_class, feature_type));
	if (it != initializers.end())
	{
		return it->second();
	}
	return nullptr;
}

