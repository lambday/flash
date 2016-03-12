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

#ifndef KERNEL_MANAGER_H__
#define KERNEL_MANAGER_H__

#include <vector>
#include <memory>
#include <shogun/lib/common.h>
#include <shogun/kernel/Kernel.h>
#include <shogun/feature/Feature.h>

namespace shogun
{

class CKernel;

namespace internal
{

class InitPerKernel
{
public:
	explicit InitPerKernel(std::shared_ptr<CKernel>& kernel) : m_kernel(kernel)
	{
	}
	~InitPerKernel()
	{
	}
	InitPerKernel& operator=(CKernel* kernel)
	{
		SG_REF(kernel);
		m_kernel = std::shared_ptr<CKernel>(kernel, [](auto& ptr) { SG_UNREF(ptr); });
		return *this;
	}
	operator const CKernel*() const
	{
		return m_kernels.get();
	}
private:
	std::shared_ptr<CKernel>& m_kernel;
};

struct PrecomputeKernel
{
	PrecomputeKernel(std::shared_ptr<CKernel>& kernel) : m_kernel(kernel)
	{
		// ASSERT that the underlying kernel is not of custom kernel or combined kernel type
		ASSERT(kernel->get_kernel_type() != K_CUSTOM);
		ASSERT(kernel->get_kernel_type() != K_COMBINED);
	}
	~PrecomputeKernel()
	{
	}
	void with_features(CFeature* feats_a, CFeatures* feats_b)
	{
		// TODO
		// create a custom kernel init-ing with the features and replace the backend kernel
		// with the precomputed one.
		m_kernel = std::shared_ptr<CKernel>(new CCustomKernel(feats_a, feats_b));
	}
	std::shared_ptr<CKernel>& m_kernel;
};

class KernelManager
{
public:
	KernelManager(index_t num_kernels) : m_num_kernels(num_kernels)
	{
		m_kernels.resize(m_num_kernels);
		std::fill(m_kernels.begin(), m_kernels.end(), nullptr);
	}
	InitPerKernel kernel_at(index_t i)
	{
		return InitPerKernel(m_kernels[i]);
	}
	PrecomputeKernel precompute_kernel_at(index_t i)
	{
		return PrecomputeKernel(m_kernels[i].get());
	}
	~KernelManager()
	{
	}
private:
	const index_t m_num_kernels;
	std::vector<std::shared_ptr<CKernel>> m_kernels;
};

}

}

#endif // KERNEL_MANAGER_H__
