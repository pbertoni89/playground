#pragma once

//
// header.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>

namespace http {
	namespace server {

		struct header
		{
			std::string name;
			std::string value;
		};

	} // namespace server
} // namespace http
