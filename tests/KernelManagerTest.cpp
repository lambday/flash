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

#include <vector>
#include <memory>
#include <algorithm>
#include <shogun/base/init.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/kernel/GaussianKernel.h>
#include <flash/statistics/internals/KernelManager.h>

using namespace shogun;
using namespace internal;

void test1()
{
	const index_t dim = 3;
	const index_t num_vec = 8;
	const index_t num_kernels = 1;

	SGMatrix<float64_t> data_p(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	data_p.display_matrix();

	auto feats = new CDenseFeatures<float64_t>(data_p);
	auto kernel = new CGaussianKernel();
	kernel->set_width(0.5);

	KernelManager mgr(num_kernels);
	const KernelManager& cmgr = mgr;

	mgr.kernel_at(0) = kernel;
	ASSERT(cmgr.kernel_at(0)->get_kernel_type() == K_GAUSSIAN);

	CKernel* k = cmgr.kernel_at(0);
	k->init(feats, feats);

	mgr.precompute_kernel_at(0);
	ASSERT(cmgr.kernel_at(0)->get_kernel_type() == K_CUSTOM);

	mgr.restore_kernel_at(0);
	ASSERT(cmgr.kernel_at(0)->get_kernel_type() == K_GAUSSIAN);
}

int main()
{
	init_shogun_with_defaults();

	test1();

	exit_shogun();
	return 0;
}
