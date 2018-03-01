/**************************************************************************************************

This example defines a routine ParallelAverage that sets output[i]
to the average of input[i-1], input[i], and input[i+1], for 1 <= i< n.

***************************************************************************************************/

#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"


struct t_3pt_average
{
	const float * input;

	float * output;

	void operator()(const tbb::blocked_range<int> & range) const
	{
		for (int i = range.begin(); i != range.end(); ++i)
		{
			output[i] = (input[i - 1] + input[i] + input[i + 1])*(1 / 3.f);
		}
	}
};


void parallel_average(float * output, const float * input, size_t n)
{
	t_3pt_average avg;
	avg.input = input;
	avg.output = output;
	tbb::parallel_for(tbb::blocked_range<int>(1, (int)n), avg);
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

	The Intel Threading Building Blocks implementation of this algorithm uses the range object
	to perform most of the steps.Predicate is_divisible performs the test in step 1, and step 2.
	The splitting constructor does steps 3 - 6. The body object does the sequential merges.

***************************************************************************************************/
#include <algorithm>


template<typename It>
struct par_merge_range
{
	static size_t grainsize;

	/**
	 * [begin1,end1) is 1st sequence to be merged
	 */
	It begin1, end1;

	/**
	 * [begin2,end2) is 2nd sequence to be merged
	 */
	It begin2, end2; // 

	/**
	 * where to put merged sequence
	 */
	It out;          // 

	bool empty() const
	{
		return (end1 - begin1) + (end2 - begin2) == 0;
	}

	bool is_divisible() const
	{
		return std::min(end1 - begin1, end2 - begin2) > grainsize;
	}

	par_merge_range(par_merge_range & r, tbb::split)
	{
		if (r.end1 - r.begin1 < r.end2 - r.begin2)
		{
			std::swap(r.begin1, r.begin2);
			std::swap(r.end1, r.end2);
		}
		It m1 = r.begin1 + (r.end1 - r.begin1) / 2;
		It m2 = std::lower_bound(r.begin2, r.end2, *m1);
		begin1 = m1;
		begin2 = m2;
		end1 = r.end1;
		end2 = r.end2;
		out = r.out + (m1 - r.begin1) + (m2 - r.begin2);
		r.end1 = m1;
		r.end2 = m2;
	}

	par_merge_range(It begin1_, It end1_,
		It begin2_, It end2_,
		It out_) :
		begin1(begin1_), end1(end1_),
		begin2(begin2_), end2(end2_), out(out_)
	{}
};


template<typename Iterator>
size_t par_merge_range<Iterator>::grainsize = 1000;


template<typename Iterator>
struct par_merge_body
{
	void operator()(par_merge_range<Iterator>& r) const
	{
		std::merge(r.begin1, r.end1, r.begin2, r.end2, r.out);
	}
};


template<typename Iterator>
void ParallelMerge(Iterator begin1, Iterator end1, Iterator begin2, Iterator end2, Iterator out)
{
	tbb::parallel_for(
		par_merge_range<Iterator>(begin1, end1, begin2, end2, out),
		par_merge_body<Iterator>(),
		tbb::simple_partitioner()
		);
}
