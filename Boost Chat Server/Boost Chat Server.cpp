#include "stdafx.h"
//
// chat_server.cpp
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
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "chat_message.hpp"

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class chat_participant
{
public:
	virtual ~chat_participant() {}
	virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_partecipant_ptr;

//----------------------------------------------------------------------

class chat_room
{
	std::set<chat_partecipant_ptr> _partecipants;
	enum { max_recent_msgs = 100 };
	chat_message_queue _recent_msgs;

public:
	size_t size()
	{
		return _partecipants.size();
	}

	void join(chat_partecipant_ptr partecipant)
	{
		_partecipants.insert(partecipant);

		for (auto msg : _recent_msgs)
		{
			partecipant->deliver(msg);
		}
	}

	void leave(chat_partecipant_ptr participant)
	{
		_partecipants.erase(participant);
	}

	void deliver(const chat_message& msg)
	{
		_recent_msgs.push_back(msg);

		while (_recent_msgs.size() > max_recent_msgs)
		{
			_recent_msgs.pop_front();
		}

		for (auto partecipant : _partecipants)
		{
			partecipant->deliver(msg);
		}
	}
};

//----------------------------------------------------------------------

class chat_session
	: public chat_participant,
	public std::enable_shared_from_this<chat_session>
{
	boost::asio::ip::tcp::socket _socket;
	chat_room & _room;
	chat_message _read_msg;
	chat_message_queue _write_msgs;

public:
	chat_session(boost::asio::ip::tcp::socket socket, chat_room & room) :
		_socket(std::move(socket)),
		_room(room)
	{
		std::cout << "Accepting client # " << (_room.size() + 1)
			<< ". Reserved port: " << _socket.remote_endpoint().port() << std::endl;
	}

	void start()
	{
		_room.join(shared_from_this());
		_do_read_header();
	}

	// Overriding chat_partecipant::deliver
	void deliver(const chat_message& msg)
	{
		bool no_one_is_writing = _write_msgs.empty();
		_write_msgs.push_back(msg);
		if (no_one_is_writing)
		{
			std::cout << "Delivering \"" << msg.to_string() << "\" to " << _socket.remote_endpoint().port() << std::endl;
			_do_write();
		}
	}

private:

	void _do_read_header()
	{
		auto self(shared_from_this());

		boost::asio::async_read(_socket,
			boost::asio::buffer(_read_msg.data(), chat_message::header_length),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec && _read_msg.decode_header())
			{
				_do_read_body();
			}
			else
			{
				_room.leave(shared_from_this());
			}
		});
	}

	void _do_read_body()
	{
		auto self(shared_from_this());

		boost::asio::async_read(_socket,
			boost::asio::buffer(_read_msg.body(), _read_msg.get_body_length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				if (_read_msg.to_string() == "COMMAND")
				{
					std::cout << "A command was issued !!!";
					_room.deliver(chat_message("RESPONSE"));
				}
				else
				{
					_room.deliver(_read_msg);
				}
				_do_read_header();				// RECURSION
			}
			else
			{
				_room.leave(shared_from_this());
			}
		});
	}

	void _do_write()
	{
		auto self(shared_from_this());

		boost::asio::async_write(_socket,
			boost::asio::buffer(_write_msgs.front().data(),
				_write_msgs.front().length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				_write_msgs.pop_front();
				if (!_write_msgs.empty())
				{
					_do_write();
				}
			}
			else
			{
				std::cout << "Exception arisen from socket " << _socket.remote_endpoint().port() << ", kicking out client" << std::endl;
				_room.leave(shared_from_this());
			}
		});
	}
};

//----------------------------------------------------------------------

class chat_server
{
	boost::asio::ip::tcp::acceptor _acceptor;
	boost::asio::ip::tcp::socket _socket;
	chat_room _room;

public:
	chat_server(boost::asio::io_service & io_service, const boost::asio::ip::tcp::endpoint & endpoint) :
		_acceptor(io_service, endpoint),
		_socket(io_service)
	{
		_do_accept();
	}

private:
	void _do_accept()
	{
		_acceptor.async_accept(_socket,
			[this](boost::system::error_code ec)
		{
			if (!ec)
			{
				std::make_shared<chat_session>(std::move(_socket), _room)->start();
				// chat_session constructed
			}

			_do_accept();
		});
	}
};

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cerr << "Usage: chat_server <port> [<port> ...]\n";
			return 1;
		}

		boost::asio::io_service io_service;

		std::list<chat_server> servers;
		for (int i = 1; i < argc; ++i)
		{
			int port = std::atoi(argv[i]);
			boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
			servers.emplace_back(io_service, endpoint);
			std::cout << "Set up server on port " << port << std::endl;
		}

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
