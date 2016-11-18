#include "stdafx.h"

#include "xspectra-asio/client.hpp"

//----------------------------------------------------------------------

#ifdef XSPECTRA_CRL
public ref class CMyStartService
{
	boost::asio::io_service * _pIoService;

public:
	CMyStartService(boost::asio::io_service * pIoService) :
		_pIoService(pIoService)
	{
	}

	void run()
	{
		_pIoService->run();
	}
};
#endif

// - - - - - - - - - - - - 

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: chat_client <host> <port>\n";
			return 1;
		}

		system("PAUSE");

		boost::asio::io_service ioService;

		boost::asio::ip::tcp::resolver resolver(ioService);
		auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });

		xspectra::asio::client client(ioService, endpoint_iterator);

#ifdef XSPECTRA_CRL
		CMyStartService^ cMyStartService = gcnew CMyStartService(&ioService);
		Thread^ thread_io_service = gcnew Thread(gcnew ThreadStart(cMyStartService, &CMyStartService::run));
		thread_io_service->Start();
#else
		std::thread thread_io_service([&ioService]() { ioService.run(); });
#endif
		char line[xspectra::asio::message::_MAX_BODY_LENGTH + 1];

		std::cout << "Start typing commands. EXIT will exit." << std::endl;
		bool bContinue = true;

		while (bContinue && std::cin.getline(line, xspectra::asio::message::_MAX_BODY_LENGTH + 1))
		{
			std::string sLine(line);

			if (sLine=="EXIT")
			{
				bContinue = false;
			}
			else
			{
				xspectra::asio::message msg(line);
				client.write(msg);
			}
		}

		client.close();

#ifdef XSPECTRA_CRL
		thread_io_service->Join();
#else
		thread_io_service.join();
#endif
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
