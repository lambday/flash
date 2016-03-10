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

#include <flash/statistics/TwoDistributionTest.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

CTwoDistributionTest::CTwoDistributionTest() : CHypothesisTest<TwoDistributionTest>()
{
}

CTwoDistributionTest::~CTwoDistributionTest()
{
}

void CTwoDistributionTest::set_p(CFeatures* samples_from_p)
{
	get_data_manager().samples_at(0) = samples_from_p;
}

void CTwoDistributionTest::set_q(CFeatures* samples_from_q)
{
	get_data_manager().samples_at(1) = samples_from_q;
}

void CTwoDistributionTest::set_num_samples_p(index_t num_samples_from_p)
{
	get_data_manager().num_samples_at(0) = num_samples_from_p;
}

void CTwoDistributionTest::set_num_samples_q(index_t num_samples_from_q)
{
	get_data_manager().num_samples_at(1) = num_samples_from_q;
}

const char* CTwoDistributionTest::get_name() const
{
	return "TwoDistributionTest";
}
