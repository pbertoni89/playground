#pragma once
/*

The code I’m going to show is an example of how to use the parallel_reduce() function (documentation),
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
It’s time now for us to have a glance at the code:

*/
#include <tbb/blocked_range.h>
#include <vector>

using namespace tbb;

class NumberPrinter
{
private:
	/** perform our local computation. i is the number to compute: display it and returns a string version of it	*/
	static char *develop(size_t i);
public:
	/**  store the output data of the computation */
	std::vector<char *> my_vect;
	
	/** Function that is called to loop through the items assigned to this instance of the class.
		TBB uses a blocked_range<size_t> to present the data to class instances.
		Here, we just call develop(i) on every item, and save the result in my_vector								*/
	void operator()(const blocked_range<size_t>& r);
	
	/** There are two constructors defined: These just match the requirements of Intel TBB to make the class run	*/

	/** copy constructor is called a split constructor,
		because it has a useless argument so that Intel TBB differentiates it from a normal copy constructor	
		
		TBB calls this split constructor when it decides that there is too much data too be handled by a single core.
		It then creates a copy of one of the class instances, and distributes the data evenly among them
		(this is entirely transparent for the user, and it’s one of the many things that makes Intel TBB so cool)	*/
	NumberPrinter(NumberPrinter& x, split);
	
	NumberPrinter(int highbit);
	
	/* Called by TBB when all the local computations are done on several processes that manage
		adjacent data in the original blocked_range. TBB will always call join(const NumberPrinter& y)
		in the order in which the data has been distributed.
		
		For instance, if processes a, b and c are all given a third of the data(in that order),
		then a will always join with b and b with c.
		This can be easily checked : just print my_vector after the joining has been done and you’ll notice
		that in there, all the elements are sorted.																	*/
	void join(const NumberPrinter& y);
};