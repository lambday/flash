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

#include <iostream>
#include <algorithm>
#include <shogun/base/init.h>
#include <shogun/lib/config.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
#include <TwoSampleTest.h>

using namespace shogun;

void test1()
{
	const index_t dim = 10;
	const index_t num_vec = 10;
	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	CTwoSampleTestBase* test = new CTwoSampleTestBase();
	test->set_p(feats_p);
	test->set_q(feats_q);

	SG_UNREF(test);
}

int main(int argc, char **argv)
{
	init_shogun_with_defaults();
	test1();
	exit_shogun();
	return 0;
}
