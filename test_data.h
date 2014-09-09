#include <shogun/lib/config.h>
#include <shogun/lib/List.h>
#include <shogun/lib/SGVector.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
#include <vector>

using namespace shogun;

// somehow this module has to provide the computation module
// the data - either blockwise streamed or as a whole fetched
//			- either permuted or unpermuted (differentiate between independene test and two-sample test)
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
template <class Features, class Fetcher, class TestDataPermutation>
struct TestDataManager
{
	using return_type = typename TestDataPermutation::return_type;

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
	template <bool IsPermutationTest>
	return_type get_samples(Fetcher fetch)
	{
		ASSERT(samples.size() > 1);

		TestDataPermutation permutation;

		for (auto sample : samples)
			permutation.samples.push_back(fetch(sample));
		return permutation.template get<IsPermutationTest>();
	}
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

// permutation policies
template <class Features>
struct TwoSampleTestPermutation
{
	typedef Features* return_type;
	template <bool IsPermutationTest> struct type {};

	template <bool IsPermutationTest>
	return_type get()
	{
		return get(type<IsPermutationTest>());
	}

	std::vector<Features*> samples;
private:
	return_type get(type<false>)
	{
		return_type p_and_q = return_type(samples[0]->create_merged_copy(samples[1]));
		return return_type(p_and_q);
	}

	return_type get(type<true>)
	{
		FeatureVectorPermutation<Features> permute_feat_vectors;
		return_type p_and_q = get<false>();
		permute_feat_vectors(p_and_q);
		return p_and_q;
	}
};

template <class Features>
struct IndependenceTestPermutation
{
	typedef std::vector<Features*> return_type;
	template <bool IsPermutationTest> struct type {};

	template <bool IsPermutationTest>
	return_type get()
	{
		return get(type<IsPermutationTest>());
	}

	std::vector<Features*> samples;

private:
	return_type get(type<false>)
	{
		return samples;
	}
	return_type get(type<true>)
	{
		FeatureVectorPermutation<Features> permute_feat_vectors;
		permute_feat_vectors(samples[0]);
		return get<false>();
	}
};

// traits
template <class Features> struct fetch_traits;

template <> template <typename T>
struct fetch_traits<CStreamingDenseFeatures<T> >
{
	typedef CStreamingDenseFeatures<T> feats_type;
	typedef CDenseFeatures<T> return_type;
};

template <> template <typename T>
struct fetch_traits<CDenseFeatures<T> >
{
	typedef CDenseFeatures<T> feats_type;
	typedef CDenseFeatures<T> return_type;
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
	using return_type = typename fetch_traits<Features>::return_type;
	using feats_type = typename fetch_traits<Features>::feats_type;

	return_type* operator()(feats_type* samples);
};

template <> template <typename T>
struct FetchBlocks<CStreamingDenseFeatures<T> >
{
	FetchBlocks(int blocksize, int num_blocks)
		: blocksize(blocksize), num_blocks(num_blocks)
	{
	}
	CDenseFeatures<T>* operator()(CStreamingDenseFeatures<T>* samples)
	{
		return (CDenseFeatures<T>*)samples->get_streamed_features(blocksize*num_blocks);
	}
	int blocksize;
	int num_blocks;
};

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


