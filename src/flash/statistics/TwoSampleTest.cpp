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

#include <shogun/kernel/Kernel.h>
#include <flash/statistics/TwoSampleTest.h>
#include <flash/statistics/internals/KernelManager.h>
#include <flash/statistics/internals/TestTypes.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

CTwoSampleTest::CTwoSampleTest() : CTwoDistributionTest(TwoSampleTest::num_kernels)
{
}

CTwoSampleTest::~CTwoSampleTest()
{
}

void CTwoSampleTest::set_kernel(CKernel* kernel)
{
	auto& km = get_kernel_manager();
	km.kernel_at(0) = kernel;
}

CKernel* CTwoSampleTest::get_kernel() const
{
	const auto& km = get_kernel_manager();
	return km.kernel_at(0);
}

const char* CTwoSampleTest::get_name() const
{
	return "TwoSampleTest";
}
