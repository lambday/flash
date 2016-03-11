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

struct InitPerKernel
{
	friend class KernelManager;
	InitPerKernel(KernelManager& km, index_t i) : kernel_manager(km), index(i)
	{
	}
	operator const CKernel*() const
	{
		return kernel_manager.m_kernels[index].get();
	}
	InitPerKernel& operator=(CKernel* kernel)
	{
		SG_REF(kernel);
		kernel_manager.m_kernels[index] = std::shared_ptr<CKernel>(kernel, [](auto& ptr) { SG_UNREF(ptr); });
		return *this;
	}
	const index_t index;
	KernelManager& kernel_manager;
};

struct PrecomputeKernel
{
	friend class KernelManager;
	PrecomputeKernel(KernelManager& km, index_t i) : kernel_manager(km), index(i)
	{
	}
	void with_features(CFeature* feats_a, CFeatures* feats_b)
	{
		// TODO
		// ASSERT that the underlying kernel is not of custom kernel or combined kernel type
		// create a custom kernel init-ing with the features and replace the backend kernel
		// with the precomputed one.
	}
	const index_t index;
	KernelManager& kernel_manager;
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
		return InitPerKernel(*this, i);
	}
	PrecomputeKernel precompute_kernel_at(index_t i)
	{
		return PrecomputeKernel(*this, i);
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
