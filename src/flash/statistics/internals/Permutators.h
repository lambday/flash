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

#ifndef PERMUTATORS_H__
#define PERMUTATORS_H__

#include <memory>
#include <shogun/lib/config.h>

namespace shogun
{

class CFeatures;
template <class T> class CDenseFeatures;

namespace internal
{

struct PermutatorBase
{
	virtual void permute(shogun::CFeatures* feats) = 0;
};

template <class Features>
class Permutator : public PermutatorBase
{
};

template <> template <typename T>
class Permutator<shogun::CDenseFeatures<T> > : public PermutatorBase
{
public:
	using feat_type = shogun::CDenseFeatures<T>;
	static std::shared_ptr<PermutatorBase> get_instance();
	virtual void permute(CFeatures* feats) override;
private:
	Permutator();
};

}

}

#endif // PERMUTATORS_H__
