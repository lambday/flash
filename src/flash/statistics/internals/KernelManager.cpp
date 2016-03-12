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
#include <shogun/kernel/Kernel.h>
#include <shogun/kernel/CustomKernel.h>
#include <flash/statistics/internals/KernelManager.h>

using namespace shogun;
using namespace internal;

KernelManager::KernelManager(index_t num_kernels)
{
	m_kernels.resize(num_kernels);
	m_precomputed_kernels.resize(num_kernels);
	std::fill(m_kernels.begin(), m_kernels.end(), nullptr);
	std::fill(m_precomputed_kernels.begin(), m_precomputed_kernels.end(), nullptr);
}

KernelManager::~KernelManager()
{
}

InitPerKernel KernelManager::kernel_at(index_t i)
{
	return InitPerKernel(m_kernels[i]);
}

CKernel* KernelManager::kernel_at(index_t i) const
{
	ASSERT(i <= m_kernels.size());
	if (m_precomputed_kernels[i] == nullptr)
	{
		return m_kernels[i].get();
	}
	return m_precomputed_kernels[i].get();
}

void KernelManager::precompute_kernel_at(index_t i)
{
	ASSERT(i <= m_kernels.size());
	auto kernel = m_kernels[i].get();
	if (kernel->get_kernel_type() != K_CUSTOM)
	{
		m_precomputed_kernels[i] = std::shared_ptr<CCustomKernel>(new CCustomKernel(kernel));
	}
}

void KernelManager::restore_kernel_at(index_t i)
{
	ASSERT(i <= m_kernels.size());
	m_precomputed_kernels[i] = nullptr;
}
