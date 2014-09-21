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

#include <shogun/lib/config.h>
#include <shogun/lib/List.h>
#include <shogun/lib/SGVector.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
#include <vector>

using namespace shogun;

// traits
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

/*
template <> template <typename T>
struct fetch_traits<CStreamingSparseFeatures<T> >
{
	typedef CStreamingSparseFeatures<T> feats_type;
	typedef CSparseFeatures<T> return_type;
};
*/

// RAII functors
template <class Features>
struct init_functor
{
	inline void operator()(Features* samples)
	{
		SG_REF(samples);
	}
};

template <> template <typename T>
struct init_functor<CStreamingDenseFeatures<T> >
{
	inline void operator()(CStreamingDenseFeatures<T>* samples)
	{
		SG_REF(samples);
		samples->start_parser();
	}
};

template <class Features>
struct cleanup_functor
{
	inline void operator()(std::vector<Features*> samples)
	{
		for (auto sample : samples)
			SG_UNREF(sample);
	}
};

template <> template <typename T>
struct cleanup_functor<CStreamingDenseFeatures<T> >
{
	inline void operator()(std::vector<CStreamingDenseFeatures<T>*> samples)
	{
		for (auto sample : samples)
		{
			sample->end_parser();
			SG_UNREF(sample);
		}
	}
};

template <class Features>
struct Data
{
	typedef Features type;
	Data(Features* _feats) : feats(_feats) {}
	~Data() { feats = NULL; }
	Features* feats;
};

template <class TestType>
struct DataStore;

template <>
struct DataStore<TwoSampleTest>
{
	std::vector<CFeatures*> samples;
};

// if the test does not need a permutation thing then we shouldn't give it any
// permutation type. it should work smoothly even without that.
// fetching is necessary - permutation is not. so probably we'd want to store
// things inside the fetcher
template <class TestType>
struct DataManager
{
	using test_type = TestType;
	using fetch_type = typename test_type::fetch_type;
	using permutation_type = typename test_type::permutation_type;
	using return_type = typename test_type::return_type;

	// passing fetch as an argument is necessary because in case of fetch
	// blocks we are supposed to call the constructor with the blocksize
	// and the number of blocks to be fetched in current burst.
	// to hide this detail from test data get_sample implementation
	// this part is left to the caller method
	// TODO
	// in order to handle different blocksize for different distribution
	// in case of different number of samples, it's wise to keep the
	// total number of blocks from all the distributions and internally
	// decide the blocksize for each distribution each time we fetch

	DataManager() {}

	// disable copy constructor and assignment operator
	DataManager(const DataManager& other);
	DataManager& operator=(const DataManager& other);

	~DataManager()
	{
		cleanup(samples);
	}

	template <class Features>
	void push_back(Features* sample)
	{
		FeatureType<Features>* feats = new FeatureType<Features>(sample);
		init(sample);
		samples.push_back(sample);
	}

	void set_permutation(bool sample_null)
	{
		IsPermutationTest = sample_null;
	}

	// TODO - we can't make this templated on IsPermutationTest
	// since this is set inside the program - DONE
	return_type get_samples(Fetcher<Features> fetch)
	{
		ASSERT(samples.size() > 1);

		permutation_type permutation;

		for (auto sample : samples)
			// passing original sample and fetcher as an arg because
			// permutation needs to know the original feature type
			// in order to free any intermediate allocated memory
			// in case streaming features is used
			permutation.push_back(fetch, sample);
		return permutation.get(IsPermutationTest);
	}

	init_functor<Features> init;
	cleanup_functor<Features> cleanup;

private:
	bool IsPermutationTest;
	DataStore<test_type> store;
};

// feature vector permutation logic
template <class Features>
struct FeatureVectorPermutation
{
	inline void operator()(Features* feats);
};

// specialization for dense features
template <> template <typename T>
struct FeatureVectorPermutation<CDenseFeatures<T> >
{
	inline void operator()(CDenseFeatures<T>* feats)
	{
		SGVector<index_t> inds(feats->get_num_vectors());
		inds.range_fill();
		inds.permute();
		inds.display_vector();
		feats->add_subset(inds);
	}
};

// specialization for sparse features
// TODO

// TestTypes

// forward declarations
template <class Features> struct TwoSampleTest;
template <class Features> struct IndependenceTest;



// these cleaners are needed since streaming from a features yields
// another type of features which is stored internally. if we don't
// keep track of these then we'll get memory leak
// it is also important that we free the streamed features immediately
// since we are already returning a merged feature object. no use of
// keeping track of it. Note that this is taken care in the destructor
// which gets called every time we call get_samples in the data manager
// long live RAII
template <class Features>
struct TwoSampleTestCleaner
{
	using feats_type = typename fetch_traits<Features>::return_type;
	TwoSampleTestCleaner(std::vector<feats_type*> samples) {}
};

template <> template <typename T>
struct TwoSampleTestCleaner<CStreamingDenseFeatures<T> >
{
	using feats_type = typename fetch_traits<CStreamingDenseFeatures<T> >::return_type;
	TwoSampleTestCleaner(std::vector<feats_type*> samples)
	{
		for (auto sample : samples)
			SG_UNREF(sample);
	}
};

// permutation policies
template <class Features>
struct TwoSampleTestPermutation
{
	typedef Features feats_type;
	using fetch_type = typename fetch_traits<Features>::return_type;
	using return_type = typename TwoSampleTest<fetch_type>::return_type;

	//template <bool IsPermutationTest> struct type {};

	~TwoSampleTestPermutation()
	{
		TwoSampleTestCleaner<feats_type> cleanup(samples);
	}

	template <template <class> class Fetcher>
	void push_back(Fetcher<feats_type> fetch, feats_type* sample)
	{
		samples.push_back(fetch(sample));
	}

	return_type get(bool IsPermutationTest)
	{
		if (IsPermutationTest)
			return get_shuffled();
		return get();
	}

private:
	return_type get()
	{
		return_type p_and_q = return_type(samples[0]->create_merged_copy(samples[1]));
		return p_and_q;
	}

	return_type get_shuffled()
	{
		FeatureVectorPermutation<fetch_type> permute_feat_vectors;
		return_type p_and_q = get();
		permute_feat_vectors(p_and_q);
		return p_and_q;
	}

	std::vector<fetch_type*> samples;
};

template <class Features>
struct IndependenceTestPermutation
{
	typedef Features feats_type;
	using fetch_type = typename fetch_traits<feats_type>::return_type;
//	typedef std::vector<fetch_type*> return_type;
	using return_type = typename IndependenceTest<fetch_type>::return_type;

	//template <bool IsPermutationTest> struct type {};

	template <template <class> class Fetcher>
	void push_back(Fetcher<feats_type> fetch, feats_type* sample)
	{
		samples.push_back(fetch(sample));
	}

	return_type get(bool IsPermutationTest)
	{
		if (IsPermutationTest)
			return get_shuffled();
		return get();
	}
private:
	return_type get()
	{
		return samples;
	}
	return_type get_shuffled()
	{
		FeatureVectorPermutation<fetch_type> permute_feat_vectors;
		permute_feat_vectors(samples[0]);
		return get();
	}
	std::vector<fetch_type*> samples;
};


// TestTypes
template <class Features>
struct TwoSampleTest
{
	typedef TwoSampleTestPermutation<Features> permutation_type;
	typedef typename fetch_traits<Features>::return_type fetch_type;
	typedef fetch_type* return_type;
};

template <class Features>
struct IndependenceTest
{
	typedef IndependenceTestPermutation<Features> permutation_type;
	typedef typename fetch_traits<Features>::return_type fetch_type;
	typedef std::vector<fetch_type*> return_type;
};

//

// fetching features policies
template <class Features>
struct FetchAll
{
	Features* operator()(Features* samples)
	{
		return samples;
	}
};

template <class Features>
struct FetchBlocks
{
	using return_type = typename fetch_traits<Features>::return_type*;
	using feats_type = typename fetch_traits<Features>::feats_type*;

	return_type operator()(feats_type samples);
};

/*
template <class StreamingFeatures>
struct ParserRAII
{
	ParserRAII(StreamingFeatures* samples) : samples(samples)
	{
		samples->start_parser();
	}
	~ParserRAII()
	{
		samples->end_parser();
	}
	StreamingFeatures* samples;
};
*/

template <> template <typename T>
struct FetchBlocks<CStreamingDenseFeatures<T> >
{
	using return_type = typename fetch_traits<CStreamingDenseFeatures<T> >::return_type*;
	using feats_type = typename fetch_traits<CStreamingDenseFeatures<T> >::feats_type*;

	FetchBlocks(int blocksize, int num_blocks) : blocksize(blocksize), num_blocks(num_blocks)
	{
	}
	return_type operator()(feats_type samples)
	{
		//ParserRAII<CStreamingDenseFeatures<T> > parser(samples);
		return return_type(samples->get_streamed_features(blocksize * num_blocks));
	}
	int blocksize;
	int num_blocks;
};

/*
template <> template <typename T>
struct FetchBlocks<CDenseFeatures<T> >
{
	FetchBlocks(int blocksize, int num_blocks)
		: blocksize(blocksize), num_blocks(num_blocks)
	{
	}
	CDenseFeatures<T>* operator()(CDenseFeatures<T>* samples)
	{
		FetchBlocks<CStreamingDenseFeatures<T> > fetch(blocksize, num_blocks);
		return fetch(new CStreamingDenseFeatures<T>(samples));
	}
	int blocksize;
	int num_blocks;
};
*/
