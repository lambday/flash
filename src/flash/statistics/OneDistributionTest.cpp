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

#include <flash/statistics/OneDistributionTest.h>
#include <flash/statistics/internals/DataManager.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

template <typename T>
COneDistributionTest<T>::COneDistributionTest() : CHypothesisTest<T>()
{
}

template <typename T>
COneDistributionTest<T>::~COneDistributionTest()
{
}

template <typename T>
void COneDistributionTest<T>::set_samples(CFeatures* samples)
{
	auto& dm = CHypothesisTest<T>::get_data_manager();
	dm.samples_at(0) = samples;
}

template <typename T>
CFeatures* COneDistributionTest<T>::get_samples() const
{
	const auto& dm = CHypothesisTest<T>::get_data_manager();
	return dm.samples_at(0);
}

template <typename T>
void COneDistributionTest<T>::set_num_samples(index_t num_samples)
{
	auto& dm = CHypothesisTest<T>::get_data_manager();
	dm.num_samples_at(0) = num_samples;
}

template <typename T>
index_t COneDistributionTest<T>::get_num_samples() const
{
	const auto& dm = CHypothesisTest<T>::get_data_manager();
	return dm.num_samples_at(0);
}

template <typename T>
const char* COneDistributionTest<T>::get_name() const
{
	return "OneDistributionTest";
}

template class COneDistributionTest<GoodnessOfFitTest>;
