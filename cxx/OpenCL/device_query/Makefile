# http://www.drdobbs.com/parallel/a-gentle-introduction-to-opencl/231002854?pgno=3
#
# https://stackoverflow.com/questions/21301905/compiling-an-opencl-program-using-a-cl-cl-h-file
# https://devtalk.nvidia.com/default/topic/975944/ubuntu-16-10-cuda-toolkit-install-with-opencl/
# But more useful are
#	$ find /usr/ -name cl.h
# 	/usr/local/cuda-10.0/targets/x86_64-linux/include/CL/cl.h
#
#	$ find /usr/ -name libOpenCL.so*
#	/usr/local/cuda-10.0/targets/x86_64-linux/lib/libOpenCL.so


PROJ=device_query

CC=g++

CFLAGS=-std=c++11 -Wall -DUNIX -g -DDEBUG

# Check for 32-bit vs 64-bit
PROC_TYPE = $(strip $(shell uname -m | grep 64))
 
# Check for Mac OS
OS = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
DARWIN = $(strip $(findstring DARWIN, $(OS)))

# MacOS System
ifneq ($(DARWIN),)
	CFLAGS += -DMAC
	LIBS=-framework OpenCL

	ifeq ($(PROC_TYPE),)
		CFLAGS+=-arch i386
	else
		CFLAGS+=-arch x86_64
	endif
else
	# Linux OS
	LIBS=-lOpenCL
	ifeq ($(PROC_TYPE),)
		CFLAGS+=-m32
	else
		CFLAGS+=-m64
	endif

	# Check for Linux-AMD
	ifdef AMDAPPSDKROOT
		INC_DIRS=. $(AMDAPPSDKROOT)/include
		ifeq ($(PROC_TYPE),)
			LIB_DIRS=$(AMDAPPSDKROOT)/lib/x86
		else
			LIB_DIRS=$(AMDAPPSDKROOT)/lib/x86_64
		endif
	else
		# Check for Linux-Nvidia
		ifdef CUDA
			$(info Check for Linux-Nvidia... CUDA found)
			INC_DIRS=. $(CUDA)/OpenCL/common/inc
		else
			# Custom lines
			CL_DIR=/usr/local/cuda-10.0/targets/x86_64-linux/
			INC_DIRS=${CL_DIR}include
			LIB_DIRS=${CL_DIR}lib
		endif
	endif
endif

$(PROJ): $(PROJ).cpp
	$(CC) $(CFLAGS) -o $@ $^ $(INC_DIRS:%=-I%) $(LIB_DIRS:%=-L%) $(LIBS)

.PHONY: clean

clean:
	rm $(PROJ)
