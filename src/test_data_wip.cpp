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

#include <vector>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <shogun/base/init.h>
#include <shogun/lib/config.h>
#include <shogun/features/FeatureTypes.h>
#include <shogun/features/Features.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>

using namespace shogun;

/*
 * any implementation has been inspired by sergey lisitsyn's blog
 * http://sergey.lisitsyn.me/2014/03/28/any-struggles/
 */

template <class T>
struct feat_class
{
	static constexpr EFeatureClass value = C_ANY;
};
template <> template <class T>
struct feat_class<CDenseFeatures<T> >
{
	static constexpr EFeatureClass value = C_DENSE;
};
template <> template <class T>
struct feat_class<CStreamingDenseFeatures<T> >
{
	static constexpr EFeatureClass value = C_STREAMING_DENSE;
};
/*
template <template <class> class T, class ST>
struct feat_type
{
	static constexpr EFeatureType value = F_ANY;
};
template <> template <class T>
struct feat_type<>
{
	static constexpr EFeatureClass value = C_DENSE;
};
template <> template <class T>
struct feat_class<CStreamingDenseFeatures<T> >
{
	static constexpr EFeatureClass value = C_STREAMING_DENSE;
};
*/
struct any
{
	template <class T>
	any(T value)
	{
		class_type = feat_class<std::remove_pointer<T>>::value;
		ptr = value;
	}
	EFeatureClass class_type;
	CFeatures* ptr;
};

/*
template <EFeatureClass C, EFeatureType T>
struct feats_traits
{
	using feat_type = CFeatures;
};

template <>
struct feats_traits<C_DENSE, F_DREAL>
{
	using feat_type = CDenseFeatures<float64_t>;
};

template <>
struct feats_traits<C_STREAMING_DENSE, F_DREAL>
{
	using feat_type = CStreamingDenseFeatures<float64_t>;
};
*/

// fetch policies
struct FetchBase
{
	virtual CFeatures* fetch(CFeatures* feats) = 0;
};

template <class Features>
struct FetchAll : public FetchBase
{
	using feat_type = Features;
	virtual CFeatures* fetch(CFeatures* feats) override
	{
		feat_type* ptr = static_cast<feat_type*>(feats);
		std::cout << "normal!" << std::endl;
		return feats;
	}
};

template <class StreamingFeatures>
struct FetchBlocks : public FetchBase
{
	using feat_type = StreamingFeatures;
	virtual CFeatures* fetch(CFeatures* feats) override
	{
		feat_type* ptr = static_cast<feat_type*>(feats);
		std::cout << "streaming!" << std::endl;
		return ptr->get_streamed_features(blocksize * num_blocks_per_burst);
	}
	int blocksize;
	int num_blocks_per_burst;
};

struct TwoSampleTest
{
	template <class T>
	using fetch_type = FetchAll<T>;
};

struct StreamingTwoSampleTest
{
	template <class T>
	using fetch_type = FetchBlocks<T>;
};

template <class TestType>
struct DataManager
{
	template <typename Features>
	void push_back(Features* feats)
	{
		static_assert(std::is_base_of<CFeatures, Features>::value, "Unsupported feature type provided!\n");
		static_assert(!std::is_same<CFeatures, Features>::value, "Please provide feature with actual type!\n");

		samples.push_back(any(feats));
		fetchers.push_back(new typename TestType::template fetch_type<Features>());
	}
	void get_samples()
	{
		ASSERT(samples.size() == fetchers.size());
		for (size_t i = 0; i < samples.size(); ++i)
		{
			CDenseFeatures<float64_t> *ptr = (CDenseFeatures<float64_t>*)fetchers[i]->fetch(samples[i].ptr);
			ptr->get_feature_matrix().display_matrix();
		}
	}
	std::vector<any> samples;
	std::vector<FetchBase*> fetchers;
};

struct DataManager2
{
	void push_back(CFeatures* feats)
	{
		samples.push_back(feats);
	}
	void test()
	{
		for (auto it : samples)
		{
			EFeatureClass type = it->get_feature_class();
			switch (type)
			{
			case C_DENSE:
//				std::cout << "DenseFeatures instance" << std::endl;
				break;
			case C_STREAMING_DENSE:
//				std::cout << "StremingDenseFeatures instance" << std::endl;
				break;
			default:
//				std::cout << "Unknown instance" << std::endl;
				break;
			}
		}
	}
	std::vector<CFeatures*> samples;
};

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

	DataManager<TwoSampleTest> data_manager;
	data_manager.push_back(feats_p);
	data_manager.push_back(feats_q);

	data_manager.get_samples();
	SG_UNREF(feats_p);
	SG_UNREF(feats_q);
}
/*
void test2()
{
	const index_t dim = 10;
	const index_t num_vec = 10;
	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	CStreamingDenseFeatures<float64_t> *sfeats_p = new CStreamingDenseFeatures<float64_t>(feats_p);
	CStreamingDenseFeatures<float64_t> *sfeats_q = new CStreamingDenseFeatures<float64_t>(feats_q);

	sfeats_p->start_parser();
	sfeats_q->start_parser();

	DataManager<StreamingTwoSampleTest> data_manager;
	data_manager.push_back(new CStreamingDenseFeatures<float64_t>(feats_p));
	data_manager.push_back(new CStreamingDenseFeatures<float64_t>(feats_q));

	data_manager.fetchers[0]->blocksize = 2;
	data_manager.fetchers[1]->blocksize = 2;
	data_manager.fetchers[0]->num_blocks_per_burst = 1;
	data_manager.fetchers[1]->num_blocks_per_burst = 1;

	data_manager.get_samples();

	sfeats_p->end_parser();
	sfeats_q->end_parser();

	SG_UNREF(sfeats_p);
	SG_UNREF(sfeats_q);
}
*/
void test3()
{
	const index_t dim = 10;
	const index_t num_vec = 10;
	SGMatrix<float64_t> data_p(dim, num_vec);
	SGMatrix<float64_t> data_q(dim, num_vec);
	std::iota(data_p.matrix, data_p.matrix + dim * num_vec, 0);
	std::iota(data_q.matrix, data_q.matrix + dim * num_vec, dim * num_vec);

	CDenseFeatures<float64_t> *feats_p = new CDenseFeatures<float64_t>(data_p);
	CDenseFeatures<float64_t> *feats_q = new CDenseFeatures<float64_t>(data_q);

	DataManager2 data_manager;
	data_manager.push_back(feats_p);
	data_manager.push_back(feats_q);

	data_manager.test();
	SG_UNREF(feats_p);
	SG_UNREF(feats_q);
}

int main(int argc, char **argv)
{
	init_shogun_with_defaults();
/*
	size_t iter = 1000000;
	if (argc < 2)
	{
		std::cout << "static" << std::endl;
		for (size_t i = 0; i < iter; ++i)
			test1();
	}
	else
	{
		std::cout << "dynamic" << std::endl;
		for (size_t i = 0; i < iter; ++i)
			test2();
	}
*/
	test1();
//	test2();

	exit_shogun();
	return 0;
}
