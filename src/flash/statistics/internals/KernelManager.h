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
		return m_kernel->get();
	}
private:
	std::shared_ptr<CKernel>& m_kernel;
};

struct PrecomputeKernel
{
	friend class KernelManager;
private:
	explicit PrecomputeKernel(std::shared_ptr<CKernel>& kernel, std::shared_ptr<CCustomKernel>& precomputed_kernel)
	{
		// if already precomputed, ignore
		if (kernel->get_kernel_type() != K_CUSTOM)
		{
			precomputed_kernel = kernel;
		}
		else
		{
			m_precomputed_kernel = std::shared_ptr<CCustomKernel>(new CCustomKernel(kernel));
		}
	}
public:
	~PrecomputeKernel()
	{
	}
	opeartor const CCustomKernel*() const
	{
		return m_precomputed_kernel->get();
	}
private:
	std::shared_ptr<CCustomKernel>& m_precomputed_kernel;
};

class KernelManager
{
public:
	KernelManager(index_t num_kernels)
	{
		m_kernels.resize(num_kernels);
		m_precomputed_kernels.resize(num_kernels);
		std::fill(m_kernels.begin(), m_kernels.end(), nullptr);
		std::fill(m_precomputed_kernels.begin(), m_precomputed_kernels.end(), nullptr);
	}
	~KernelManager()
	{
	}
	InitPerKernel kernel_at(index_t i)
	{
		return InitPerKernel(m_kernels[i]);
	}
	const CKernel* kernel_at(index_t i) const
	{
		ASSERT(i <= m_kernels.size());
		if (m_precomputed_kernels[i] == nullptr)
		{
			return m_kernels[i].get();
		}
		return m_precomputed_kernels[i].get();
	}
	PrecomputeKernel precompute_kernel_at(index_t i)
	{
		return PrecomputeKernel(m_kernels[i], m_precomputed_kernels[i]);
	}
	const CKernel* restore_kernel_at(index_t i)
	{
		ASSERT(i <= m_kernels.size());
		m_precomputed_kernels[i] = nullptr;
		return m_kernels[i];
	}
private:
	std::vector<std::shared_ptr<CKernel>> m_kernels;
	std::vector<std::shared_ptr<CCustomKernel>> m_precomputed_kernels;
};

}

}

#endif // KERNEL_MANAGER_H__
