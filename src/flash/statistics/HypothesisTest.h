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

#ifndef __HYPOTHESIS_TEST_H_
#define __HYPOTHESIS_TEST_H_

#include <shogun/lib/config.h>
#include <shogun/base/SGObject.h>
#include <flash/statistics/internals/DataManager.h>
#include <memory>

namespace shogun
{

class CFeatures;

namespace statistics
{

template <class TestType>
class CHypothesisTest : public CSGObject
{
public:
	using test_type = TestType;

	CHypothesisTest();
	CHypothesisTest(const CHypothesisTest& other);
	CHypothesisTest& operator=(const CHypothesisTest& other);
	~CHypothesisTest();

	void set_p(CFeatures* p);
	void set_q(CFeatures* q);

	void set_simulate_h0(bool simulate_h0);
	bool get_simulate_h0();

	typename test_type::return_type get_samples();

	virtual const char* get_name() const;
private:
	struct Self;
	std::unique_ptr<Self> impl;
protected:
	internal::DataManager<test_type>& get_data_manager();
};

}

}

#endif // __HYPOTHESIS_TEST_H_
