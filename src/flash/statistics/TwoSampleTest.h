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

#ifndef __TWO_SAMPLE_TEST_H_
#define __TWO_SAMPLE_TEST_H_

#include <shogun/lib/config.h>
#include <flash/statistics/HypothesisTest.h>
#include <flash/statistics/internals/TestTypes.h>

namespace shogun
{

namespace statistics
{

class CTwoSampleTest : public CHypothesisTest<internal::TwoSampleTest>
{
public:
	using test_type = CHypothesisTest<internal::TwoSampleTest>::test_type;

	CTwoSampleTest();
	CTwoSampleTest(const CTwoSampleTest& other);
	~CTwoSampleTest();
	virtual const char* get_name() const;
};

}

}
#endif // __TWO_SAMPLE_TEST_H_