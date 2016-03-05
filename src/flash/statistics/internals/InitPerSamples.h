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

#ifndef INIT_PER_SAMPLES_H__
#define INIT_PER_SAMPLES_H__

#include <shogun/lib/common.h>

namespace shogun
{

class CFeatures;

namespace internal
{

template <class T> class DataManager;

template <typename TestType>
struct InitPerSamples
{
	explicit InitPerSamples(DataManager<TestType>& dm, index_t );
	InitPerSamples& operator=(CFeatures* feats);
	~InitPerSamples();
	index_t index;
	DataManager<TestType>& data_manager;
};
}

}

#endif // INIT_PER_SAMPLES_H__

