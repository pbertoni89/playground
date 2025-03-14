#include <iostream>
#include <cstdint>
#include <iomanip>
#include <format>

/**
 *
 * g++ --std=c++20 -O0 unions.cpp
 *
 * DEEPSEEK
 * Purpose of Bit-Fields:
 * Bit-fields are used to pack multiple fields into a single integer type (in this case, a uint64_t). This is often done to:
 * 	Save Memory: By tightly packing data, you can reduce memory usage.
 * 	Match Hardware or Protocol Requirements: Many hardware devices or communication protocols use specific bit-widths for data fields.
 * 	For example, ADC (Analog-to-Digital Converter) data might be represented as 16-bit values.
 *
 * Layout of the Union:
 * The union allows you to access the same 64-bit memory in two ways:
 * 	As Individual Fields: Through the fields struct, where each field is explicitly defined with a specific bit-width.
 * 	As a Single 64-bit Value: Through the value member, which represents the entire 64-bit memory as a single integer.
 * The total size of the fields struct is 64 bits (16 + 4 + 16 + 16 + 4 + 16 = 72, but due to alignment and packing, it fits into 64 bits).
*/


using t_f = uint64_t;
using t_v = uint64_t;

// values > 16 will print "warning: width of ‘oscope_t::<unnamed struct>::b’ exceeds its type"
// without this, sizeof(oscope_t) == 8
// with    this between [ 1, 16], sizeof(oscope_t) == 10, NO MATTER WHAT THE VALUE
// with    this between [16, 16], sizeof(oscope_t) == 12, NO MATTER WHAT THE VALUE
#define BREAK_ALIGNMENT 4


union oscope_t
{
	struct {
		t_f a : 16;    // 16
#ifdef BREAK_ALIGNMENT
		t_f b : BREAK_ALIGNMENT;
#endif
		t_f c : 16;    // 32
		t_f d : 16;    // 48
		t_f e : 16;    // 64
	} fields;

	t_v value;
};


union virgin_t {
	struct {
		uint64_t a : 16;
		//@UNUSEDuint64_t b : 4;  // FIXME uncommenting it would breaks the union bitmask
		uint64_t c : 16;
		uint64_t d : 16;
		//@UNUSEDuint64_t digital_tracks_1 : 4;  // FIXME uncommenting it would breaks the union bitmask
		uint64_t e : 16;
	} fields;

	uint64_t value;
};


std::string fff (const t_f v) {
	std::stringstream os;
	os
		<< ": 0b" << std::setfill('0') << std::setw(64) << std::format("{:b}", v)
		<< ", 0d" << std::setfill('0') << std::setw(16) << v
		<< ", 0x" << std::hex << std::setfill('0') << std::setw(16) << v << std::dec;
	return os.str();
}


std::ostream & operator<< (std::ostream & os, const oscope_t & osc)
{
	return os << ": oscope_t {\n"
		<< "  a" << fff(osc.fields.a)<< "\n"
#ifdef BREAK_ALIGNMENT
		<< "  b" << fff(osc.fields.b)<< "\n"
#endif
		<< "  c" << fff(osc.fields.c)<< "\n"
		<< "  d" << fff(osc.fields.d)<< "\n"
		<< "  e" << fff(osc.fields.e)<< "\n"
		<< "  _" << fff(osc.value)<< "\n"
		<< "}\n";
}


int main()
{
	/// using u = virgin_t;
	using u = oscope_t;
	u osc{};
	std::cout << "union has a stack size of " << sizeof(osc) << "\ni " << osc << std::endl;

	osc.fields.a = 1 << 8;
	// std::cout << "a " << osc << std::endl;

#ifdef BREAK_ALIGNMENT
	osc.fields.b = 0b1010;
	std::cout << "b " << osc << std::endl;
#endif

	osc.fields.c = 54321;
	std::cout << "c " << osc << std::endl;

	osc.fields.d = 12345;
	std::cout << "d " << osc << std::endl;

	osc.fields.e = 7 << 4;
	std::cout << "e " << osc << std::endl;
    return 0;
}

