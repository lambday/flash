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

#ifndef NEXT_SAMPLES_H__
#define NEXT_SAMPLES_H__

#include <vector>
#include <memory>
#include <shogun/lib/common.h>

namespace shogun
{

class CFeatures;

namespace internal
{

/**
 * @brief class NextSamples is the return type for next() call in DataManager.
 * If there are no more samples (from any one of the distributions), an empty
 * instance of NextSamples is supposed to be returned. This can be verified
 * from the caller by calling the empty() method. Otherwise, always a get()
 * call with appropriate index would give the samples from that distribution.
 * If an inappropriate index is provided, e.g. get(2) for a two-sample test,
 * a runtime exception is thrown.
 *
 * Example usage:
 * @code
 * 		NextSamples next_samples(2);
 * 		next_samples.at(0) = fetchers[0].next();
 * 		next_samples.at(1) = fetchers[1].next();
 * 		if (!next_samples.empty())
 * 		{
 * 			auto first = next_samples.get(0);
 * 			auto second = next_samples.get(1);
 * 			auto third = next_samples.get(2); // Runtime Error
 * 		}
 * @endcode
 */
class NextSamples
{
public:
	NextSamples(const index_t num_distributions);
	std::shared_ptr<CFeatures>& at(index_t i);
	std::shared_ptr<CFeatures> get(index_t i); // try const
	const bool empty() const;
private:
	const index_t m_num_distributions;
	std::vector<std::shared_ptr<CFeatures>> next_samples;
};

}

}

#endif // NEXT_SAMPLES_H__
