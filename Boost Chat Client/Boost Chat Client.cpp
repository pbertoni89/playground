#include "stdafx.h"

//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <boost/asio.hpp>
#include "..\Boost Chat Server\chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
	boost::asio::io_service& _io_service;
	tcp::socket _socket;
	chat_message _read_msg;
	chat_message_queue _write_msgs;

public:
	chat_client(boost::asio::io_service & io_service, tcp::resolver::iterator endpoint_iterator) :
		_io_service(io_service),
		_socket(io_service)
	{
		do_connect(endpoint_iterator);
	}

	void write(const chat_message& msg)
	{
		_io_service.post(
			[this, msg]()
		{
			bool write_in_progress = !_write_msgs.empty();
			_write_msgs.push_back(msg);
			if (!write_in_progress)
			{
				do_write();
			}
		});
	}

	void close()
	{
		_io_service.post([this]() { _socket.close(); });
	}

private:
	void do_connect(tcp::resolver::iterator endpoint_iterator)
	{
		boost::asio::async_connect(_socket, endpoint_iterator,
			[this](boost::system::error_code ec, tcp::resolver::iterator)
		{
			if (!ec)
			{
				do_read_header();
			}
		});
	}

	void do_read_header()
	{
		boost::asio::async_read(_socket,
			boost::asio::buffer(_read_msg.data(), chat_message::header_length),
			[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec && _read_msg.decode_header())
			{
				do_read_body();
			}
			else
			{
				_socket.close();
			}
		});
	}

	void do_read_body()
	{
		boost::asio::async_read(_socket,
			boost::asio::buffer(_read_msg.body(), _read_msg.get_body_length()),
			[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				std::cout << "Receiving: \"";
				std::cout.write(_read_msg.body(), _read_msg.get_body_length());
				std::cout << "\"\n";
				do_read_header();
			}
			else
			{
				_socket.close();
			}
		});
	}

	void do_write()
	{
		boost::asio::async_write(_socket,
			boost::asio::buffer(_write_msgs.front().data(),
				_write_msgs.front().length()),
			[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				_write_msgs.pop_front();
				if (!_write_msgs.empty())	// there is at leaste another message in queue: recursion
				{
					do_write();
				}
			}
			else
			{
				_socket.close();
			}
		});
	}
};

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: chat_client <host> <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
		chat_client client(io_service, endpoint_iterator);

		std::thread thread_io_service([&io_service]() { io_service.run(); });

		char line[chat_message::max_body_length + 1];

		while (std::cin.getline(line, chat_message::max_body_length + 1))
		{
			std::string sLine(line);

			chat_message msg(line);
			client.write(msg);
		}

		client.close();
		thread_io_service.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
