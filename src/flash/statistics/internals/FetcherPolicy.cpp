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

#include <memory>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
#include <flash/statistics/internals/FetcherPolicy.h>

using namespace std;
using namespace shogun;
using namespace internal;

template <class T>
AllFetcher<T>::AllFetcher() : FetcherBase()
{
}

template <class T>
shared_ptr<FetcherBase> AllFetcher<T>::get_instance()
{
	static shared_ptr<FetcherBase> instance(new AllFetcher<T>);
	return instance;
}

template <class T>
CFeatures* AllFetcher<T>::fetch(CFeatures* feats)
{
	T* ptr = static_cast<T*>(feats);
	SG_REF(ptr);
	return ptr;
}

template <class T>
BlockFetcher<T>::BlockFetcher() : FetcherBase()
{
}

template <class T>
shared_ptr<FetcherBase> BlockFetcher<T>::get_instance()
{
	shared_ptr<FetcherBase> instance(new BlockFetcher<T>);
	return instance;
}

template <class T>
CFeatures* BlockFetcher<T>::fetch(CFeatures* feats)
{
	T* ptr = static_cast<T*>(feats);
	return ptr->get_streamed_features(blocksize * num_blocks_per_burst);
}

template struct AllFetcher<CDenseFeatures<float64_t>>;
template struct BlockFetcher<CStreamingDenseFeatures<float64_t>>;