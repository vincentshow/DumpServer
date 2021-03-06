#include "Poco\LocalDateTime.h"
#include "Poco\DateTimeFormatter.h"
#include "Poco\Util\HelpFormatter.h"
#include "Poco\Util\ServerApplication.h"
#include "Poco\Net\ServerSocket.h"
#include "Poco\Net\HTTPServer.h"
#include "Poco\Data\SQLite\Connector.h"

#include "DumpServer.h"
#include "FormRequestHandlerFactory.h"

using namespace std;
using namespace Poco::Util;
using namespace Poco::Net;
using Poco::Data::SQLite::Connector;

void DumpServer::initialize(Application& self)
{
	// register SQLite connector
	Connector::registerConnector();

	loadConfiguration(); // load default configuration files, if present
	ServerApplication::initialize(self);
}

void DumpServer::uninitialize()
{
	ServerApplication::uninitialize();
}

void DumpServer::defineOptions(OptionSet& options)
{
	ServerApplication::defineOptions(options);

	options.addOption(Option("help", "h", "display help information on command line arguments")
		.required(false)
		.repeatable(false));
}

void DumpServer::handleOption(const string& name, const string& value)
{
	ServerApplication::handleOption(name, value);

	if (name == "help")
		_helpRequested = true;
}

void DumpServer::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.setHeader("A web server that shows how to work with HTML forms.");
	helpFormatter.format(std::cout);
}

int DumpServer::main(const std::vector<std::string>& args)
{
	if (_helpRequested)
	{
		displayHelp();
	}
	else
	{
		int tid = Poco::Thread::currentTid();
		unsigned short port = (unsigned short)config().getInt("HTTPFormServer.port", 9980);
		string basePath = config().getString("HTTPFORMServer.basePath", "dumps\\");

		// set-up a server socket
		ServerSocket svs(port);
		// set-up a HTTPServer instance
		HTTPServer srv(new FormRequestHandlerFactory(basePath), svs, new HTTPServerParams);
		// start the HTTPServer
		srv.start();
		this->logger().information(Poco::format("%s: Server startup in tid %d", Poco::DateTimeFormatter::format(Poco::LocalDateTime().timestamp(), "%Y-%m-%d %H:%M:%s"), tid));
		// wait for CTRL-C or kill
		waitForTerminationRequest();
		// Stop the HTTPServer
		srv.stop();
	}
	return Application::EXIT_OK;
}
