/*
 * Copyright (c) 2009-2017, Farooq Mela
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// https://gist.github.com/fmela/591333#file-stacktrace-cxx-L1


// build with g++ -rdynamic -ggdb backtrace.cpp -ldl


#include <execinfo.h> // for backtrace
#include <dlfcn.h>    // for dladdr
#include <cxxabi.h>   // for __cxa_demangle

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cassert>
#include <exception>
#include <iostream>


// This function produces a stack backtrace with demangled function & method names.
std::string Backtrace(int skip = 1)
{
    void *callstack[128];
    const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
    char buf[1024];
    int nFrames = backtrace(callstack, nMaxFrames);
    char **symbols = backtrace_symbols(callstack, nFrames);

    std::ostringstream trace_buf;
    for (int i = skip; i < nFrames; i++)
    {
        printf("%s\n", symbols[i]);

        Dl_info info;
        if (dladdr(callstack[i], &info) and info.dli_sname)
        {
            char *demangled = NULL;
            int status = -1;
            if (info.dli_sname[0] == '_')
                demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);

            snprintf(buf, sizeof(buf), "%-3d %*p %s + %zd\n",
                     i, int(2 + sizeof(void*) * 2), callstack[i],
                     status == 0 ? demangled :
                     info.dli_sname == 0 ? symbols[i] : info.dli_sname,
                     (char *)callstack[i] - (char *)info.dli_saddr);
            free(demangled);
        }
        else
        {
            snprintf(buf, sizeof(buf), "%-3d %*p %s\n",
                     i, int(2 + sizeof(void*) * 2), callstack[i], symbols[i]);
        }
        trace_buf << buf;
    }
    free(symbols);

    if (nFrames == nMaxFrames)
        trace_buf << "[truncated]\n";

    return trace_buf.str();
}



int show_bt()
{
	int a = 3;
	Backtrace();
	a++;
}



void fatal(const std::exception & E)
{
    std::cerr << "do - something - extremely - important\n";
    throw;
}


int culprit()
{
    throw std::runtime_error("yet another foo function");
}

int suspected()
{
	return culprit();
}


int custom_trycatch()
{
    try
    {
        return suspected();
    }
    catch (const std::exception & E)
    {
        //fatal(E);
					std::cerr << "catched! " << E.what() << std::endl;
					throw;
    }
    return -1;
}


/**
 * run with
 * 
 * g++ custom-bt.cpp -rdynamic -ggdb -ldl
 * ulimit -c unlimited
 * custom-bt
 * ~/xspectra-dev/xspyctra/xspyctrad.py -b custom-bt
 */
int main()
{
	// culprit(); ok
	// suspected(); ok
	custom_trycatch();

	double b;
	show_bt();
	return 31;
}
