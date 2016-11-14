#include "tbb/parallel_for.h"
using namespace tbb;

/**************************************************************************************************

This example defines a routine ParallelAverage that sets output[i]
to the average of input[i-1], input[i], and input[i+1], for 1 <= i< n.

***************************************************************************************************/
#include "tbb/blocked_range.h"

struct Average
{
	const float* input;

	float* output;

	void operator()(const blocked_range<int>& range) const
	{
		for (int i = range.begin(); i != range.end(); ++i)
			output[i] = (input[i - 1] + input[i] + input[i + 1])*(1 / 3.f);
	}
};

// Note: Reads input[0..n] and writes output[1..n-1]. 
void ParallelAverage(float* output, const float* input, size_t n)
{
	Average avg;
	avg.input = input;
	avg.output = output;
	parallel_for(blocked_range<int>(1, (int)n), avg);
}

/**************************************************************************************************

This example is more complex and requires familiarity with STL.
It shows the power of parallel_for beyond flat iteration spaces.
The code performs a parallel merge of two sorted sequences.
It works for any sequence with a random - access iterator.
The algorithm(Akl 1987) works recursively as follows :

	1) If the sequences are too short for effective use of parallelism, do a sequential merge.
		Otherwise perform steps 2 - 6.

	2) Swap the sequences if necessary, so that the first sequence[begin1, end1) is at least
		as long as the second sequence[begin2, end2).

	3) Set m1 to the middle position in[begin1, end1). Call the item at that location key.
	4) Set m2 to where key would fall in[begin2, end2).
	5) Merge[begin1, m1) and [begin2, m2) to create the first part of the merged sequence.
	6) Merge[m1, end1) and [m2, end2) to create the second part of the merged sequence.

	The Intel® Threading Building Blocks implementation of this algorithm uses the range object
	to perform most of the steps.Predicate is_divisible performs the test in step 1, and step 2.
	The splitting constructor does steps 3 - 6. The body object does the sequential merges.

***************************************************************************************************/
#include <algorithm>

template<typename Iterator>
struct ParallelMergeRange
{
	static size_t grainsize;
	Iterator begin1, end1; // [begin1,end1) is 1st sequence to be merged
	Iterator begin2, end2; // [begin2,end2) is 2nd sequence to be merged
	Iterator out;               // where to put merged sequence

	bool empty() const
	{
		return (end1 - begin1) + (end2 - begin2) == 0;
	}

	bool is_divisible() const
	{
		return std::min(end1 - begin1, end2 - begin2) > grainsize;
	}

	ParallelMergeRange(ParallelMergeRange& r, split)
	{
		if (r.end1 - r.begin1 < r.end2 - r.begin2)
		{
			std::swap(r.begin1, r.begin2);
			std::swap(r.end1, r.end2);
		}
		Iterator m1 = r.begin1 + (r.end1 - r.begin1) / 2;
		Iterator m2 = std::lower_bound(r.begin2, r.end2, *m1);
		begin1 = m1;
		begin2 = m2;
		end1 = r.end1;
		end2 = r.end2;
		out = r.out + (m1 - r.begin1) + (m2 - r.begin2);
		r.end1 = m1;
		r.end2 = m2;
	}

	ParallelMergeRange(Iterator begin1_, Iterator end1_,
		Iterator begin2_, Iterator end2_,
		Iterator out_) :
		begin1(begin1_), end1(end1_),
		begin2(begin2_), end2(end2_), out(out_)
	{}
};

template<typename Iterator>
size_t ParallelMergeRange<Iterator>::grainsize = 1000;

template<typename Iterator>
struct ParallelMergeBody
{
	void operator()(ParallelMergeRange<Iterator>& r) const
	{
		std::merge(r.begin1, r.end1, r.begin2, r.end2, r.out);
	}
};

template<typename Iterator>
void ParallelMerge(Iterator begin1, Iterator end1, Iterator begin2, Iterator end2, Iterator out)
{
	parallel_for(
		ParallelMergeRange<Iterator>(begin1, end1, begin2, end2, out),
		ParallelMergeBody<Iterator>(),
		simple_partitioner()
		);
}
