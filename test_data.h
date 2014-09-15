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

// TODO solve memory leaks - DONE
// TODO solve different block-size issue
// TODO parserRAII does not work. (stopping and starting it again starts
// it from the beginning) - DONE
// TODO make TestDataManager un-copiable (since it has to perform RAII
// action) - DONE
// TODO need to make sure that get_samples() always returns the features
// (or vector containing) features with refcount 1 and all other internal
// storage is cleaned up so that no external cleanup is required. - DONE

// DEPENDENCIES:
// - It relies on the fact that CDenseFeatures->create_merged_copy() always
// returns a new object with refcount 1
// - It relies on the fact that CStreamingDenseFeatures->get_streamed_features()
// also returns a new object with refcount 1
// TODO think about ways if we can get rid of these dependencies as well.
// Or possible drawbacks due to these dependencies
// TODO think about ways to do things similarly for all streaming dot features
// subclasses - at least dense, sparse and string so that we don't have to write
// code for each of these explicitly.

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

// somehow this module has to provide the computation module
// the data - either blockwise streamed or as a whole fetched
//          - either permuted or unpermuted (differentiate between independene
//            test and two-sample test)
// or
// the kernel - doesn't bother about how the data was fetched
// always return a precomputed kernel (dense or sparse doesn't matter)
// use index permutation for permutation test
//
// provide the computation module either 1. data or 2. kernel

// test data main class
// instead of using concrete classes as template params, use selectors
// for ease of using the API - ensure compatibility
// info:
// fetcher is dependent on features
// test data permutation is independent of features
template <class Features, template <class> class Fetcher, template <class> class TestDataPermutation>
struct TestDataManager
{
//	using fetch_type = typename fetch_traits<Features>::return_type;
	using return_type = typename TestDataPermutation<Features>::return_type;

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

	TestDataManager() {}

	// disable copy constructor and assignment operator
	TestDataManager(const TestDataManager& other);
	TestDataManager& operator=(const TestDataManager& other);

	~TestDataManager()
	{
		cleanup(samples);
	}

	void push_back(Features* sample)
	{
		init(sample);
		samples.push_back(sample);
	}
	template <bool IsPermutationTest>
	return_type get_samples(Fetcher<Features> fetch)
	{
		ASSERT(samples.size() > 1);

		TestDataPermutation<Features> permutation;

		for (auto sample : samples)
			// passing original sample and fetcher as an arg because
			// permutation needs to know the original feature type
			// in order to free any intermediate allocated memory
			// in case streaming features is used
			permutation.push_back(fetch, sample);
		return permutation.template get<IsPermutationTest>();
	}

	init_functor<Features> init;
	cleanup_functor<Features> cleanup;

private:
	std::vector<Features*> samples;
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
	typedef fetch_type* return_type;

	template <bool IsPermutationTest> struct type {};

	~TwoSampleTestPermutation()
	{
		TwoSampleTestCleaner<feats_type> cleanup(samples);
	}

	template <template <class> class Fetcher>
	void push_back(Fetcher<feats_type> fetch, feats_type* sample)
	{
		samples.push_back(fetch(sample));
	}

	template <bool IsPermutationTest>
	return_type get()
	{
		return get(type<IsPermutationTest>());
	}

private:
	return_type get(type<false>)
	{
		return_type p_and_q = return_type(samples[0]->create_merged_copy(samples[1]));
		return p_and_q;
	}

	return_type get(type<true>)
	{
		FeatureVectorPermutation<fetch_type> permute_feat_vectors;
		return_type p_and_q = get<false>();
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
	typedef std::vector<fetch_type*> return_type;

	template <bool IsPermutationTest> struct type {};

	template <template <class> class Fetcher>
	void push_back(Fetcher<feats_type> fetch, feats_type* sample)
	{
		samples.push_back(fetch(sample));
	}

	template <bool IsPermutationTest>
	return_type get()
	{
		return get(type<IsPermutationTest>());
	}
private:
	return_type get(type<false>)
	{
		return samples;
	}
	return_type get(type<true>)
	{
		FeatureVectorPermutation<fetch_type> permute_feat_vectors;
		permute_feat_vectors(samples[0]);
		return get<false>();
	}
	std::vector<fetch_type*> samples;
};


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
