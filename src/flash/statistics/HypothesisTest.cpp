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

using namespace shogun;
using namespace internal;
using namespace flash;

template <class T>
CHypothesisTest<T>::CHypothesisTest() : CSGObject()
{
}

template <class T>
CHypothesisTest<T>::CHypothesisTest(const CHypothesisTest<T>& other)
	: CSGObject(), data_manager(other.data_manager)
{
}

template <class T>
CHypothesisTest<T>& CHypothesisTest<T>::operator=(const CHypothesisTest<T>& other)
{
	data_manager = other.data_manager;
	return *this;
}

template <class T>
CHypothesisTest<T>::~CHypothesisTest()
{
}

template <class T>
void CHypothesisTest<T>::set_p(CFeatures* p)
{
	set_feats(p);
}

template <class T>
void CHypothesisTest<T>::set_q(CFeatures* q)
{
	set_feats(q);
}

template <class T>
void CHypothesisTest<T>::set_simulate_h0(bool simulate_h0)
{
	data_manager.set_simulate_h0(simulate_h0);
}

template <class T>
bool CHypothesisTest<T>::get_simulate_h0()
{
	return data_manager.get_simulate_h0();
}

template <class T>
typename T::return_type CHypothesisTest<T>::get_samples()
{
	return data_manager.get_samples();
}

template <class T>
const char* CHypothesisTest<T>::get_name() const
{
	return "HypothesisTest";
}

template <class T>
void CHypothesisTest<T>::set_feats(CFeatures* feats)
{
	// TODO - think of better ways to do this, maybe demangling (cxxabi)
	if (feats->get_feature_type() == F_DREAL)
	{
		if (feats->get_feature_class() == C_DENSE)
			data_manager.push_back(static_cast<typename feats_traits<C_DENSE,F_DREAL>::type*>(feats));
		else if (feats->get_feature_class() == C_STREAMING_DENSE)
			data_manager.push_back(static_cast<typename feats_traits<C_STREAMING_DENSE,F_DREAL>::type*>(feats));
		else
			SG_SNOTIMPLEMENTED;
	}
	else
		SG_SNOTIMPLEMENTED;
}

template class CHypothesisTest<TwoSampleTest>;
template class CHypothesisTest<IndependenceTest>;
