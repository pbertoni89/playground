
__kernel void SAXPY(__global float* x, // this is global memory, or simply memory allocated on the device
					__global float* y,
					float a)
{
	/* 
		In order to identify the kernel instance, the runtime environment provides an id.
		Inside the kernel, we use get_global_id() which returns the id of the current work item in the first dimension.
		We will start as many instances as there are elements in our vector, and each work item will process exactly one entry.
	*/
	const int i = get_global_id(0);

	y[i] += a * x[i];
}

/*
	The basic assumption is that many instances of the kernel are executed in parallel, each processing a single work item.
	Multiple work items are executed together as part of a work group.
	Inside a work group, each kernel instance can communicate with other instances.
	This is the only execution ordering guarantee that OpenCL gives you;
	there is no specified order how work-items inside a group are processed.
	
	The work-group execution order is also undefined.
	In fact, you cannot tell if the items are executed in parallel, sequential, or in random order.

	Work-groups allow for some order as all items in a work-group can be synchronized. (not used here)
*/