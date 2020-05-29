#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <bitset>
#include <vector>


typedef enum
{
	NI_OK = 0x0,
	NI_ERROR_A = 0x1,
	NI_ERROR_B = 0x2,
	NI_ERROR_C = 0x4
} e_return;


typedef enum
{
	DU_OK = 0x0,
	DU_ERR = 0x1
} e_warning;


using t_ni_enum_bs = std::bitset<32>;


template<typename E>
class TestBitMask
{
public:
	class Flag
	{
	public:
		const int m_code;
		const std::string m_lbl;
		const t_ni_enum_bs m_bs;

		Flag(int code, std::string && lbl) :
			m_code(code), m_lbl(lbl), m_bs(code)
		{}
	};

private:
	std::vector<Flag> m_flags;

public:
	
	TestBitMask() = default;

	void add(const Flag & f)
	{
		m_flags.emplace_back(f);
	}

	TestBitMask(std::initializer_list<Flag> flags)
	{
		for (auto f : flags)
		{
			m_flags.emplace_back(f);
		}
	}

	std::string test(const E & e) const
	{
		std::stringstream ss;
		const t_ni_enum_bs BS_ALL(e);

		for (const auto & f : m_flags)
			if ((BS_ALL & f.m_bs).any())
				ss << " + " << f.m_lbl;

		return ss.str();
	};
};


void dump(const e_return & e)
{
	std::stringstream ss;
	ss << "return: " << e;
									// TestBitMask<e_return> TBM0;
									// TBM0.add({NI_OK, "NI_OK"});
	const TestBitMask<e_return> TBM{{NI_OK, "NI_OK"}, {NI_ERROR_A, "NI_ERROR_A"}, {NI_ERROR_B, "NI_ERROR_B"}, {NI_ERROR_C, "NI_ERROR_C"}};
	ss << TBM.test(e);

	std::cout << ss.str() << std::endl;
}


void dump(const e_warning & e)
{
	std::stringstream ss;
	ss << "warning: " << e;

	const TestBitMask<e_warning> TBM{{DU_OK, "DU_OK"}, {DU_ERR, "DU_ERR"}};
	ss << TBM.test(e);

	std::cout << ss.str() << std::endl;
}


int main(int argc, char**argv)
{
	e_return rv = NI_ERROR_A;
	dump(rv);
	rv = NI_ERROR_B | NI_ERROR_C;
	dump(rv);
	rv = NI_OK;
	dump(rv);
	rv = NI_ERROR_B | NI_ERROR_C | NI_ERROR_A;
	dump(rv);
	e_warning wr = DU_ERR;
	dump(wr);

	// constexpr TestBitMask<e_return> cetbm;
}

