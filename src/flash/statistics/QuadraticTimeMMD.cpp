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

#include <flash/statistics/QuadraticTimeMMD.h>

using namespace shogun;
using namespace internal;
using namespace statistics;

CQuadraticTimeMMD::CQuadraticTimeMMD() : CMMD<CQuadraticTimeMMD>()
{
}

CQuadraticTimeMMD::~CQuadraticTimeMMD()
{
}

internal::mmd::FullDirect CQuadraticTimeMMD::get_direct_estimation_method()
{
	static internal::mmd::FullDirect method;
	return method;
}

const char* CQuadraticTimeMMD::get_name() const
{
	return "QuadraticTimeMMD";
}
