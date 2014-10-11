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
#include <iostream> // TODO remove
#include <shogun/lib/config.h>
#include <shogun/lib/SGVector.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <flash/statistics/internals/Permutators.h>

using namespace std;
using namespace shogun;
using namespace internal;

template <> template <typename T>
Permutator<CDenseFeatures<T>>::Permutator()
{
}

template <> template <typename T>
shared_ptr<PermutatorBase> Permutator<CDenseFeatures<T>>::get_instance()
{
	typedef CDenseFeatures<T> feat_type;
	static shared_ptr<PermutatorBase> instance(new Permutator<feat_type>);
	return instance;
}

template <> template <typename T>
void Permutator<CDenseFeatures<T>>::permute(CFeatures* feats)
{
	std::cout << "permuting the feature vectors" << std::endl;
	typedef CDenseFeatures<T> feat_type;
	feat_type* casted_feats = static_cast<feat_type*>(feats);
	casted_feats->remove_all_subsets();
	SGVector<index_t> inds = SGVector<index_t>::randperm_vec(feats->get_num_vectors());
	casted_feats->add_subset(inds);
}

template class Permutator<CDenseFeatures<float64_t>>;
