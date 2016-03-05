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
#include <flash/statistics/internals/Permutators.h>
#include <flash/statistics/internals/FetcherPolicy.h>
#include <flash/statistics/internals/FetcherFactory.h>
#include <flash/statistics/internals/PermutatorFactory.h>
#include <flash/statistics/internals/PermutationPolicy.h>
#include <flash/statistics/internals/TestTypes.h>
#include <shogun/features/Features.h>

using namespace shogun;
using namespace internal;

template <class TestType>
InitPerSamples<TestType>::InitPerSamples(DataManager<TestType>& dm, index_t i) : data_manager(dm)
{
	std::cout << "InitPerSamples::Constructor() with i = " << i << std::endl;
	ASSERT(i < data_manager.samples.size());
	index = i;
}

template <class TestType>
InitPerSamples<TestType>::~InitPerSamples()
{
	std::cout << "InitPerSamples::Destructor()" << std::endl;
}

template <class TestType>
InitPerSamples<TestType>& InitPerSamples<TestType>::operator=(CFeatures* feats)
{
	std::cout << "InitPerSamples::Assignment() : setting the samples, fetchers and permutators" << std::endl;
	SG_REF(feats);
	data_manager.samples[index] = std::shared_ptr<CFeatures>(feats, [](CFeatures* f) { SG_UNREF(f); });
	data_manager.fetchers[index] = FetcherFactory::get_instance(feats);
	data_manager.permutation_policy->permutators[index] = PermutatorFactory::get_instance(feats);
	return *this;
}

template class InitPerSamples<TwoSampleTest>;
template class InitPerSamples<StreamingTwoSampleTest>;
template class InitPerSamples<IndependenceTest>;
template class InitPerSamples<StreamingIndependenceTest>;
