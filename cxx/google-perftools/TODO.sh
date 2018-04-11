
# apt-get install google-perftools
#
# https://github.com/gperftools/gperftools
#

#See docs/heapprofile.html for information about how to use tcmalloc's
#heap profiler and analyze its output.

#As a quick-start, do the following after installing this package:

#1) Link your executable with -ltcmalloc
#2) Run your executable with the HEAPPROFILE environment var set:
     #$ HEAPPROFILE=/tmp/heapprof <path/to/binary> [binary args]
#3) Run pprof to analyze the heap usage
     #$ pprof <path/to/binary> /tmp/heapprof.0045.heap  # run 'ls' to see options
     #$ pprof --gv <path/to/binary> /tmp/heapprof.0045.heap

#You can also use LD_PRELOAD to heap-profile an executable that you
#didn't compile.

#There are other environment variables, besides HEAPPROFILE, you can
#set to adjust the heap-profiler behavior; c.f. "ENVIRONMENT VARIABLES"
#below.

#The heap profiler is available on all unix-based systems we've tested;
#see INSTALL for more details.  It is not currently available on Windows.

binName="./a.out"
heapFile="/tmp/heapprof.0045.heap"

g++ main.cpp -o ${binName} -ltcmalloc
HEAPPROFILE=/tmp/heapprof ${binName}
# echo "# # # # # # # # # # # # # #"
# pprof ${binName} ${heapFile}
echo "# # # # # # # # # # # # # #"
pprof --gv ${binName} ${heapFile}
