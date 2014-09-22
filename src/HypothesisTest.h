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
#include <DataManager.h>

namespace shogun
{

// test types
struct TwoSampleTest
{
	template <class T>
	using fetch_type = FetchAll<T>;
};

struct IndependenceTest
{
	template <class T>
	using fetch_type = FetchAll<T>;
};

struct StreamingTwoSampleTest
{
	template <class T>
	using fetch_type = FetchBlocks<T>;
};

struct StreamingIndependenceTest
{
	template <class T>
	using fetch_type = FetchBlocks<T>;
};

// hypothesis test base class
template <class TestType>
class CHypothesisTestBase : public CSGObject
{
public:
	using test_type = TestType;

	CHypothesisTestBase() {}
	CHypothesisTestBase(const CHypothesisTestBase& other) : data_manager(other.data_manager) {}
	CHypothesisTestBase& operator=(const CHypothesisTestBase& other)
	{
		data_manager = other.data_manager;
		return *this;
	}
	~CHypothesisTestBase() {}

	// these are provided for the API sanity
	template <class Features>
	inline void set_p(Features* p)
	{
		set_feats(p);
	}

	template <class Features>
	inline void set_q(Features* q)
	{
		set_feats(q);
	}

	virtual const char* get_name() const
	{
		return "HypothesisTestBase";
	}
protected:
	template <class Features>
	inline void set_feats(Features* feats)
	{
		data_manager.push_back(feats);
	}

	DataManager<test_type> data_manager;
};

}

#endif // __HYPOTHESIS_TEST_H_
