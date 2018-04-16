#pragma once

/*
The code I'm going to show is an example of how to use the parallel_reduce() function (documentation),
which is useful for parallelizing loops when you will perform a task on a set of data but where the order
of processing is not relevant. For instance, when searching through an unordered set.

The idea is somehow similar to the Divide and Conquer approach used in serial algorithms:
TBB is going to split the set into subsets (called ranges) that are reasonably calculable by a single CPU,
and then join the results of the subsets back together in order to return the result of the whole original set.

My sample class is pretty simple: it prints every number of a given set (from 1 to 10000) to the screen.
What we will do in the parallel version is print these numbers when each range is being processed.

The example was chosen because it illustrates a side-effect of using multiple cores:
the order of processing every element is not preserved, which is exactly why parallel_reduce should only be used
for code where every iteration is independent from others. Indeed, you will very probably see that the numbers
are not being printed in their lexicographical order, as would have in a serial program.

However, a noticeable thing here is that results are joined in the same order than if the algorithm had been ran with a single CPU.
This is easy to check: when you print the numbers during the joining operation, they will all appear in the lexicographical order.
It's time now for us to have a glance at the code:
*/

#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>

/**
 * https://software.intel.com/en-us/node/506063
 */
void example_parallel_reduce_simple(int n);

void example_parallel_reduce_number_printer(int n);

/**
 * https://software.intel.com/en-us/node/506064
 */
void example_parallel_reduce_min_value(int n);
