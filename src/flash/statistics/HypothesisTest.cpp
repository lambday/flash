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
 * but WITHOUT ANY WARRANTY; without even the selfied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <flash/statistics/HypothesisTest.h>
#include <flash/statistics/internals/TestTypes.h>
#include <flash/statistics/internals/DataManager.h>
#include <flash/statistics/internals/KernelManager.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

template <class T>
struct CHypothesisTest<T>::Self
{
	Self() : data_manager(num_distributions), kernel_manager(num_kernels)
	{
	}
	DataManager data_manager;
	KernelManager kernel_manager;
};

template <class T>
CHypothesisTest<T>::CHypothesisTest() : CSGObject()
{
	self = std::make_unique<Self>();
}

template <class T>
DataManager& CHypothesisTest<T>::get_data_manager()
{
	return self->data_manager;
}

template <class T>
const DataManager& CHypothesisTest<T>::get_data_manager() const
{
	return self->data_manager;
}

template <class T>
KernelManager& CHypothesisTest<T>::get_kernel_manager()
{
	return self->kernel_manager;
}

template <class T>
const KernelManager& CHypothesisTest<T>::get_kernel_manager() const
{
	return self->kernel_manager;
}

template <class T>
CHypothesisTest<T>::~CHypothesisTest()
{
}

template <class T>
const char* CHypothesisTest<T>::get_name() const
{
	return "HypothesisTest";
}

template class CHypothesisTest<GoodnessOfFitTest>;
template class CHypothesisTest<TwoSampleTest>;
template class CHypothesisTest<IndependenceTest>;
