#include <shogun/lib/config.h>
#include <shogun/lib/List.h>
#include <shogun/lib/SGVector.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/features/streaming/StreamingDenseFeatures.h>
#include <vector>

using namespace shogun;

// TODO solve memory leaks
// TODO solve different block-size issue
// TODO parserRAII does not work. (stopping and starting it again starts it from the beginning)

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
template <class Features, template <class> class Fetcher, template <class> class TestDataPermutation>
struct TestDataManager
{
	using fetch_type = typename fetch_traits<Features>::return_type;
	using return_type = typename TestDataPermutation<fetch_type>::return_type;

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
	return_type get_samples(Fetcher<Features> fetch)
	{
		ASSERT(samples.size() > 1);

		TestDataPermutation<fetch_type> permutation;

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
// TODO


// permutation policies
template <class Features>
struct TwoSampleTestPermutation
{
	typedef Features feats_type;
	typedef Features* return_type;

	template <bool IsPermutationTest> struct type {};

	template <bool IsPermutationTest>
	return_type get()
	{
		return get(type<IsPermutationTest>());
	}

	std::vector<feats_type*> samples;
private:
	return_type get(type<false>)
	{
		return_type p_and_q = return_type(samples[0]->create_merged_copy(samples[1]));
		return p_and_q;
	}

	return_type get(type<true>)
	{
		FeatureVectorPermutation<feats_type> permute_feat_vectors;
		return_type p_and_q = get<false>();
		permute_feat_vectors(p_and_q);
		return p_and_q;
	}
};

template <class Features>
struct IndependenceTestPermutation
{
	typedef Features feats_type;
	typedef std::vector<Features*> return_type;

	template <bool IsPermutationTest> struct type {};

	template <bool IsPermutationTest>
	return_type get()
	{
		return get(type<IsPermutationTest>());
	}

	std::vector<feats_type*> samples;

private:
	return_type get(type<false>)
	{
		return samples;
	}
	return_type get(type<true>)
	{
		FeatureVectorPermutation<feats_type> permute_feat_vectors;
		permute_feat_vectors(samples[0]);
		return get<false>();
	}
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

template <> template <typename T>
struct FetchBlocks<CStreamingDenseFeatures<T> >
{
	FetchBlocks(int blocksize, int num_blocks)
		: blocksize(blocksize), num_blocks(num_blocks)
	{
	}
	// TODO handle start_parser() and end_parser() in some smart way
	CDenseFeatures<T>* operator()(CStreamingDenseFeatures<T>* samples)
	{
		ParserRAII<CStreamingDenseFeatures<T> > parser(samples);
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


