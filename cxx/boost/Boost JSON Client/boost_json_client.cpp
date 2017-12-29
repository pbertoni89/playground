// Boost JSON Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <boost/asio.hpp>
#include <iostream>

#define MY_SERVER_PORT 8888

/** http://stackoverflow.com/questions/26761058/how-to-send-http-request-and-retrieve-a-json-response-c-boost	*/
void send_json_example_request()
{
	boost::system::error_code ec;

	// what we need
	boost::asio::io_service svc;
	boost::asio::ip::tcp::socket sock(svc);
	sock.connect({ {}, MY_SERVER_PORT });

	// send request
	std::string request("GET /newGame?name=david HTTP/1.1\r\n\r\n");
	sock.send(boost::asio::buffer(request));

	// read response
	std::string response;

	do
	{
		char buf[1024];
		size_t bytes_transferred = sock.receive(boost::asio::buffer(buf), {}, ec);
		if (!ec)
		{
			response.append(buf, buf + bytes_transferred);
		}
	} while (!ec);

	std::cout << "Response received: '" << response << "'\n";
}

// Check http://stackoverflow.com/questions/24428293/reading-json-from-a-socket-using-boostasio too

int main()
{
	send_json_example_request();
}