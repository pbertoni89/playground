//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


constexpr size_t SZ_BUF = 1024;


int main(int argc, char* argv[])
{
    char request[SZ_BUF], reply[SZ_BUF];
    if (argc != 3)
    {
        std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
        return 1;
    }

    try
    {
        boost::asio::io_context io_context;
        tcp::socket s(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(s, resolver.resolve(argv[1], argv[2]));

        std::cout << "Enter message: ";
        std::cin.getline(request, SZ_BUF);
        const size_t SZ_REQ = std::strlen(request);
        boost::asio::write(s, boost::asio::buffer(request, SZ_REQ));

        const size_t SZ_REP = boost::asio::read(s, boost::asio::buffer(reply, SZ_REQ));
        std::cout << "Reply is: ";
        std::cout.write(reply, SZ_REP);
        std::cout << "\n";

        // s.close(); // explicit yet unnecessary
    }
    catch (const std::exception & e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}

