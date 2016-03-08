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

#include <algorithm>
#include <flash/statistics/internals/NextSamples.h>
#include <shogun/features/Features.h>

using namespace shogun;
using namespace internal;

NextSamples::NextSamples(const index_t num_distributions)
: m_num_distributions(num_distributions)
{
	next_samples.resize(m_num_distributions);
	std::fill(next_samples.begin(), next_samples.end(), nullptr);
}

std::shared_ptr<CFeatures>& NextSamples::at(index_t i)
{
	REQUIRE(i >= 0 && i < m_num_distributions,
			"index (%d) must be between [0,%d]!\n", i, m_num_distributions - 1);
	return next_samples[i];
}

std::shared_ptr<CFeatures> NextSamples::get(index_t i)
{
	REQUIRE(i >= 0 && i < m_num_distributions,
			"index (%d) must be between [0,%d]!\n", i, m_num_distributions - 1);
	return next_samples[i];
}

const bool NextSamples::empty() const
{
	return std::any_of(next_samples.cbegin(), next_samples.cend(), [](const auto& ptr) { return ptr == nullptr; });
}
