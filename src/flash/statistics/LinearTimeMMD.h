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

#ifndef LINEAR_TIME_MMD_H_
#define LINEAR_TIME_MMD_H_

#include <flash/statistics/MMD.h>
#include <flash/statistics/internals/mmd/WithinBlockDirect.h>

namespace shogun
{

namespace statistics
{

class CLinearTimeMMD : public CMMD<CLinearTimeMMD>
{
	friend class CMMD;
public:
	CLinearTimeMMD();
	virtual ~CLinearTimeMMD();
	virtual const char* get_name() const;
private:
	static internal::mmd::WithinBlockDirect get_direct_estimation_method();
};

}

}
#endif // LINEAR_TIME_MMD_H_
