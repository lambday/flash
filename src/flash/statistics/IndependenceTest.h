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

#ifndef __INDEPENDENCE_TEST_H_
#define __INDEPENDENCE_TEST_H_

#include <shogun/lib/config.h>
#include <flash/statistics/HypothesisTest.h>
#include <flash/statistics/internals/TestTypes.h>

namespace shogun
{

namespace statistics
{

class CIndependenceTest : public CHypothesisTest<internal::IndependenceTest>
{
public:
	using test_type = CHypothesisTest<internal::IndependenceTest>::test_type;

	CIndependenceTest();
	~CIndependenceTest();
	virtual const char* get_name() const;
};

}

}
#endif // __INDEPENDENCE_TEST_H_
