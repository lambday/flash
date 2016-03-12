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

#include <flash/statistics/TwoDistributionTest.h>
#include <flash/statistics/internals/DataManager.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

template <typename T>
CTwoDistributionTest<T>::CTwoDistributionTest() : CHypothesisTest<T>()
{
}

template <typename T>
CTwoDistributionTest<T>::~CTwoDistributionTest()
{
}

template <typename T>
void CTwoDistributionTest<T>::set_p(CFeatures* samples_from_p)
{
	DataManager& dm = CHypothesisTest<T>::get_data_manager();
	dm.samples_at(0) = samples_from_p;
}

template <typename T>
const CFeatures* CTwoDistributionTest<T>::get_p() const
{
	const DataManager& dm = CHypothesisTest<T>::get_data_manager();
	return dm.samples_at(0);
}

template <typename T>
void CTwoDistributionTest<T>::set_q(CFeatures* samples_from_q)
{
	DataManager& dm = CHypothesisTest<T>::get_data_manager();
	dm.samples_at(1) = samples_from_q;
}

template <typename T>
const CFeatures* CTwoDistributionTest<T>::get_q() const
{
	const DataManager& dm = CHypothesisTest<T>::get_data_manager();
	return dm.samples_at(1);
}

template <typename T>
void CTwoDistributionTest<T>::set_num_samples_p(index_t num_samples_from_p)
{
	DataManager& dm = CHypothesisTest<T>::get_data_manager();
	dm.num_samples_at(0) = num_samples_from_p;
}

template <typename T>
const index_t CTwoDistributionTest<T>::get_num_samples_p() const
{
	const DataManager& dm = CHypothesisTest<T>::get_data_manager();
	return dm.num_samples_at(0);
}

template <typename T>
void CTwoDistributionTest<T>::set_num_samples_q(index_t num_samples_from_q)
{
	DataManager& dm = CHypothesisTest<T>::get_data_manager();
	dm.num_samples_at(1) = num_samples_from_q;
}

template <typename T>
const index_t CTwoDistributionTest<T>::get_num_samples_q() const
{
	const DataManager& dm = CHypothesisTest<T>::get_data_manager();
	return dm.num_samples_at(1);
}

template <typename T>
const char* CTwoDistributionTest<T>::get_name() const
{
	return "TwoDistributionTest";
}

template class CTwoDistributionTest<TwoSampleTest>;
template class CTwoDistributionTest<IndependenceTest>;
