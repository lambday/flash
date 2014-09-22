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

#include <iostream>
#include <type_traits>
#include <shogun/lib/config.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>

#ifndef FETCH_POLICY_H_
#define FETCH_POLICY_H_

using namespace shogun;

struct FetchBase
{
	virtual CFeatures* fetch(CFeatures* feats) = 0;
};

// TODO - make these dense<T> and streamingdense<T>
// should not compile if feat types provided other than these
template <class Features>
struct FetchAll : public FetchBase
{
	static_assert(std::is_base_of<CFeatures, Features>::value,
			"Unsupported feature type provided!\n");

	using feat_type = Features;
	virtual CFeatures* fetch(CFeatures* feats) override
	{
		feat_type* ptr = static_cast<feat_type*>(feats);
		std::cout << "normal!" << std::endl;
		return feats;
	}
};

template <class Features>
struct FetchBlocks : public FetchBase
{
	static_assert(std::is_base_of<CStreamingFeatures, Features>::value,
			"Unsupported feature type provided!\n");

	using feat_type = Features;
	virtual CFeatures* fetch(CFeatures* feats) override
	{
		feat_type* ptr = static_cast<feat_type*>(feats);
		std::cout << "streaming!" << std::endl;
		return ptr->get_streamed_features(blocksize * num_blocks_per_burst);
	}
	int blocksize;
	int num_blocks_per_burst;
};

#endif // FETCH_POLICY_H_
