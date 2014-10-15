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

#include <shogun/lib/config.h>
#include <flash/statistics/StreamingTwoSampleTest.h>
#include <flash/statistics/internals/TestTypes.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

CStreamingTwoSampleTest::CStreamingTwoSampleTest() : CHypothesisTest<StreamingTwoSampleTest>()
{
}

CStreamingTwoSampleTest::CStreamingTwoSampleTest(const CStreamingTwoSampleTest& other)
	: CHypothesisTest<StreamingTwoSampleTest>(other)
{
}

CStreamingTwoSampleTest::~CStreamingTwoSampleTest()
{
}

void CStreamingTwoSampleTest::set_blocksize(index_t blocksize)
{
	data_manager.set_blocksize(blocksize);
}

void CStreamingTwoSampleTest::set_num_blocks_per_burst(index_t num_blocks_per_burst)
{
	data_manager.set_num_blocks_per_burst(num_blocks_per_burst);
}

const char* CStreamingTwoSampleTest::get_name() const
{
	return "StreamingTwoSampleTest";
}
