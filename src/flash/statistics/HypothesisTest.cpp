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

#include <shogun/lib/config.h>
#include <shogun/io/SGIO.h>
#include <shogun/features/Features.h>
#include <flash/statistics/HypothesisTest.h>
#include <flash/statistics/internals/TestTypes.h>
#include <flash/statistics/internals/Features.h>
#include <memory>

using namespace shogun;
using namespace internal;
using namespace statistics;

template <class T>
struct CHypothesisTest<T>::Self
{
	Self() {}
	Self(DataManager<T> data_mgr) : data_manager(data_mgr) {}
	DataManager<T> data_manager;
};

template <class T>
CHypothesisTest<T>::CHypothesisTest() : CSGObject()
{
	impl = std::unique_ptr<Self>(new Self);
}

template <class T>
CHypothesisTest<T>::CHypothesisTest(const CHypothesisTest<T>& other)
	: CSGObject()
{
	impl = std::unique_ptr<Self>(new Self(other.impl->data_manager));
}

template <class T>
CHypothesisTest<T>& CHypothesisTest<T>::operator=(const CHypothesisTest<T>& other)
{
	impl = std::unique_ptr<Self>(new Self(other.impl->data_manager));
	return *this;
}

template <class T>
DataManager<T>& CHypothesisTest<T>::get_data_manager()
{
	return impl->data_manager;
}

template <class T>
CHypothesisTest<T>::~CHypothesisTest()
{
}

template <class T>
void CHypothesisTest<T>::set_p(CFeatures* p)
{
	impl->data_manager.put(p, 0);
}

template <class T>
void CHypothesisTest<T>::set_q(CFeatures* q)
{
	impl->data_manager.put(q, 1);
}

template <class T>
void CHypothesisTest<T>::set_simulate_h0(bool simulate_h0)
{
	impl->data_manager.set_simulate_h0(simulate_h0);
}

template <class T>
bool CHypothesisTest<T>::get_simulate_h0()
{
	return impl->data_manager.get_simulate_h0();
}

template <class T>
typename T::return_type CHypothesisTest<T>::get_samples()
{
	return impl->data_manager.get_samples();
}

template <class T>
const char* CHypothesisTest<T>::get_name() const
{
	return "HypothesisTest";
}

template class CHypothesisTest<TwoSampleTest>;
template class CHypothesisTest<StreamingTwoSampleTest>;
template class CHypothesisTest<IndependenceTest>;
template class CHypothesisTest<StreamingIndependenceTest>;
