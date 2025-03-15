#pragma once

#ifdef _WIN32
#define L64(x) x##i64
#else
#define L64(x) x##LL
#endif


int main_resultset_types(int argc, const char **argv);
