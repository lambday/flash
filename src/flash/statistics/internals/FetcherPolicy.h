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

#include <memory>
#include <shogun/lib/config.h>

#ifndef FETCHER_POLICY_H__
#define FETCHER_POLICY_H__

namespace shogun
{

class CFeatures;
class CStreamingFeatures;
template <class T> class CDenseFeatures;
template <class T> class CStreamingDenseFeatures;

namespace internal
{

template <class Features>
struct fetch_traits
{
	typedef Features feats_type;
	typedef Features return_type;
};

template <> template <typename T>
struct fetch_traits<CStreamingDenseFeatures<T> >
{
	typedef CStreamingDenseFeatures<T> feats_type;
	typedef CDenseFeatures<T> return_type;
};

struct FetcherBase
{
	virtual CFeatures* fetch(CFeatures* feats) = 0;
};

template <class Features>
class AllFetcher : public FetcherBase
{
	static_assert(std::is_base_of<CFeatures, Features>::value,
			"Unsupported feature type provided!\n");
	static_assert(!std::is_base_of<CStreamingFeatures, Features>::value,
			"Not supported for streaming features!\n");
public:
	using feat_type = Features;
	static std::shared_ptr<FetcherBase> get_instance();
	virtual CFeatures* fetch(CFeatures* feats) override;
private:
	AllFetcher();
};

template <class Features>
class BlockFetcher : public FetcherBase
{
	static_assert(std::is_base_of<CStreamingFeatures, Features>::value,
			"Unsupported feature type provided!\n");
public:
	using feat_type = Features;
	static std::shared_ptr<FetcherBase> get_instance();
	virtual CFeatures* fetch(CFeatures* feats) override;
	int blocksize;
	int num_blocks_per_burst;
private:
	BlockFetcher();
};

}

}
#endif // FETCHER_POLICY_H__
