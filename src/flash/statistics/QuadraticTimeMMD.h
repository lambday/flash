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

#ifndef QUADRATIC_TIME_MMD_H_
#define QUADRATIC_TIME_MMD_H_

#include <flash/statistics/MMD.h>
#include <flash/statistics/internals/mmd/FullDirect.h>

namespace shogun
{

namespace statistics
{

class CQuadraticTimeMMD : public CMMD<CQuadraticTimeMMD>
{
	friend class CMMD;
public:
	CQuadraticTimeMMD();
	virtual ~CQuadraticTimeMMD();
	virtual const char* get_name() const;
private:
	static internal::mmd::FullDirect get_direct_estimation_method();
	const float64_t normalize_statistic(float64_t statistic) const;
	const float64_t normalize_variance(float64_t variance) const;
};

}

}
#endif // QUADRATIC_TIME_MMD_H_
