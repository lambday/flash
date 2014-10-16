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

#ifndef __STREAMING_TWO_SAMPLE_TEST_H_
#define __STREAMING_TWO_SAMPLE_TEST_H_

#include <shogun/lib/config.h>
#include <flash/statistics/HypothesisTest.h>
#include <flash/statistics/internals/TestTypes.h>

namespace shogun
{

namespace statistics
{

class CStreamingTwoSampleTest : public CHypothesisTest<internal::StreamingTwoSampleTest>
{
public:
	using test_type = CHypothesisTest<internal::StreamingTwoSampleTest>::test_type;

	CStreamingTwoSampleTest();
	CStreamingTwoSampleTest(const CStreamingTwoSampleTest& other);
	~CStreamingTwoSampleTest();

	void set_num_samples_p(index_t num_samples_p);
	void set_num_samples_q(index_t num_samples_q);

	void set_blocksize(index_t blocksize);
	void set_num_blocks_per_burst(index_t num_blocks_per_burst);

	virtual const char* get_name() const;
};

}

}
#endif // __STREAMING_TWO_SAMPLE_TEST_H_
