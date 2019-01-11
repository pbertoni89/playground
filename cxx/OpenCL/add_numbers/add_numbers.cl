// #define XN_VERBOSE


__kernel void add_numbers(
							__global float4 * data,
							__local  float  * local_result,
							__global float  * group_result)
{
	float sum;

	/*
	 * float4 is a vector type and it's similar to an array of four floats, but
	 * all four floats are operated upon in the same clock cycle.
	 * (if the target device supports vector operations)
	 */
	float4 input1, input2, sum_vector;

	const int gid = get_global_id(0), lid = get_local_id(0);

	/*
	 * provide the work-item with its global ID, which distinguishes it from all
	 * the other executing work-items. With this ID, it computes global_addr,
	 * the address in global memory from which it should load data
	 */
	const int global_addr = gid * 2, local_addr = lid;

	/*
	 * do the actual work
	 */
	input1 = data[global_addr];
	input2 = data[global_addr + 1];
	sum_vector = input1 + input2;

	/*
	 * Store local results (the classroom blackboard)
	 */
	local_result[local_addr] =
							sum_vector.s0 + sum_vector.s1 +
							sum_vector.s2 + sum_vector.s3;

	if (gid == 0)
	{
		printf("Running OpenCL %d ...\n", __OPENCL_VERSION__);
	}
#ifdef XN_VERBOSE
										printf("kernel gid %2d, lid %2d, gad %2d, computed %.2f\n",
											gid, lid, global_addr, local_result[local_addr]);
#endif
	barrier(CLK_LOCAL_MEM_FENCE);

	if (lid == 0)
	{
		sum = 0.0f;
		for (int i=0; i<get_local_size(0); i++)
		{
			sum += local_result[i];
		}
		group_result[get_group_id(0)] = sum;
#ifdef XN_VERBOSE
										printf("lid 0, group %2d summed %.2f\n", get_group_id(0), sum);
#endif
	}
}
