#pragma once

#include "Stdafx.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

namespace xspectra
{
	public class unman_message
	{
	public:

		enum { header_length = 4 };
		enum { max_body_length = 512 };

	private:

		char _data[header_length + max_body_length];
		std::size_t _body_length;

	public:

		unman_message();
		unman_message(const char * sBody);

		const char * data() const;

		char * data();

		std::size_t length() const;

		const char * body() const;

		char * body();

		const std::string to_string() const;

		std::size_t get_body_length() const;

		void set_body_length(std::size_t new_length);

		bool decode_header();

		void encode_header();
	};
}
