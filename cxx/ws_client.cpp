#include <simple-websocket-server/client_ws.hpp>
#include <memory>
#include <string>


int main()
{
	using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;
	using SpMessage =  std::shared_ptr<WsClient::InMessage>;
	using SpConn = std::shared_ptr<WsClient::Connection>;

	// TODO check what happens if not found because DSPU serves at cfg::def::PORT_WS; IPU serves at cfg::def::PORT_WS_DVR
	WsClient client("localhost:8889/dvr");
	uint nFailedAttempts = 0;

	// timeout is dangerous now that DVR can block for a long time
	// client.config.timeout_request = 4;  // on request handling
	// client.config.timeout_idle = 4;     // on idle

	std::string sCommand = "mycmd", sResponse;

	client.on_open = [& sCommand] (SpConn connection)
	{
		std::cout << "on_open: " << sCommand << std::endl;
		connection->send(sCommand);
	};

	client.on_message = [& sResponse] (SpConn connection, SpMessage in_message)
	{
		sResponse = in_message->string();
		std::cout << "on_message: " << sResponse << std::endl;
		connection->send_close(1000, "<ds satisfied>");
	};

	client.on_error = [&nFailedAttempts] (SpConn /*connection*/, const SimpleWeb::error_code & ec)
	{
		nFailedAttempts ++;
		std::cerr << "on_error: " << ec << ", error message: " << ec.message() << std::endl;
	};

	client.on_close = [] (SpConn conn, int status, const std::string & reason)
	{
		std::cout << "on_close: " << conn.get() << " with code " << status << ", reason " << reason << std::endl;
	};

	client.start();
	std::cout << "client started" << std::endl;
	client.stop();
	std::cout << "client stopped" << std::endl;

	std::cout << "exit program. Faileds: " << nFailedAttempts << "\n";
}
