#include "stdafx.h"

#include "xspectra-asio/server.hpp"

//#define NOT_SELF_CONTAINED_SERVER_USAGE

int main(int argc, char* argv[])
{
#ifdef NOT_SELF_CONTAINED_SERVER_USAGE
	boost::asio::io_service ioService;
	std::list<xspectra::asio::server*> servers;

	try
	{
		if (argc < 2)
		{
			std::cerr << "Usage: server <port> [<port> ...]\n";
			return 1;
		}


		for (int i = 1; i < argc; ++i)
		{
			int port = std::atoi(argv[i]);
			boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

			xspectra::asio::server * newServer = new xspectra::asio::server(ioService, endpoint);
			servers.emplace_back(newServer);
			std::cout << "Set up server on port " << port << std::endl;
		}

		ioService.run();

		std::cout << "Server is running." << std::endl;

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	servers.clear();

#else
	if (argc < 2)
	{
		std::cerr << "Usage: server <port>\n";
		return 1;
	}

	xspectra::asio::server newServer(std::atoi(argv[0]));
	newServer.start();
	// HERE IS THE RUNTIME
	newServer.join();
#endif
	return 0;
}
