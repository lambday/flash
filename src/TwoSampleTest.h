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
#include <HypothesisTest.h>

namespace shogun
{

// two-sample test base class
class CTwoSampleTestBase : public CHypothesisTestBase<TwoSampleTest>
{
public:
	using test_type = CHypothesisTestBase<TwoSampleTest>::test_type;

	CTwoSampleTestBase() : CHypothesisTestBase<TwoSampleTest>() {}
	CTwoSampleTestBase(const CTwoSampleTestBase& other) : CHypothesisTestBase<TwoSampleTest>(other) {}
	~CTwoSampleTestBase() {}

	virtual const char* get_name() const
	{
		return "TwoSampleTestBase";
	}
};

class CKernelTwoSampleTestBase : public CTwoSampleTestBase
{
public:
	using test_type = CTwoSampleTestBase::test_type;
protected:
//	KernelManager<test_type> kernel_manager;
};

}

#endif // __TWO_SAMPLE_TEST_H_
