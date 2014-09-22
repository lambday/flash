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

#include <vector>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <shogun/lib/config.h>
#include <shogun/features/Features.h>
#include <FetchPolicy.h>

#ifndef DATA_MANAGER_H_
#define DATA_MANAGER_H_

using namespace shogun;

template <class TestType>
struct DataManager
{
	template <typename Features>
	void push_back(Features* feats)
	{
		static_assert(std::is_base_of<CFeatures, Features>::value, "Unsupported feature type provided!\n");
		static_assert(!std::is_same<CFeatures, Features>::value, "Please provide feature with actual type!\n");

		samples.push_back(feats);
		fetchers.push_back(new typename TestType::template fetch_type<Features>());
	}
	void get_samples()
	{
		ASSERT(samples.size() == fetchers.size());
		for (size_t i = 0; i < samples.size(); ++i)
		{
			// just for test TODO remove
			CDenseFeatures<float64_t> *ptr = (CDenseFeatures<float64_t>*)fetchers[i]->fetch(samples[i]);
			ptr->get_feature_matrix().display_matrix();
		}
	}
	std::vector<CFeatures*> samples;
	std::vector<FetchBase*> fetchers;
};

#endif // DATA_MANAGER_H_
