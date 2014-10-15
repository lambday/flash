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

#include <map>
#include <memory>
#include <functional>
#include <shogun/lib/config.h>
#include <shogun/features/Features.h>

#ifndef FETCHER_FACTORY_H__
#define FETCHER_FACTORY_H__

namespace shogun
{

namespace internal
{

class FetcherBase;

struct FetcherFactory
{
	using return_type = std::shared_ptr<FetcherBase>;
	using factory = std::map<std::pair<EFeatureClass, EFeatureType>, std::function<return_type()>>;

	static return_type get_instance(CFeatures* feats);

private:
	static factory initialize();
	static const factory& initializers;
};

}

}
#endif // FETCHER_FACTORY_H__
