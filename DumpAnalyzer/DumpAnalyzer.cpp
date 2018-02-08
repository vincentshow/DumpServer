#include "Poco\LocalDateTime.h"
#include "Poco\DateTimeFormatter.h"
#include "Poco\Util\HelpFormatter.h"
#include "Poco\Util\ServerApplication.h"
#include "Poco\Data\SQLite\Connector.h"

#include "Poco/Logger.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/AutoPtr.h"

#include "DumpAnalyzer.h"
#include "WindbgProcessor.h"

using Poco::Logger;
using Poco::SimpleFileChannel;
using Poco::AutoPtr;

using namespace std;
using namespace Poco::Util;
using Poco::Data::SQLite::Connector;

void DumpAnalyzer::initialize(Application& self)
{
	// register SQLite connector
	Connector::registerConnector();

	loadConfiguration(); // load default configuration files, if present
	ServerApplication::initialize(self);
}

void DumpAnalyzer::uninitialize()
{
	ServerApplication::uninitialize();
}

void DumpAnalyzer::defineOptions(OptionSet& options)
{
	ServerApplication::defineOptions(options);

	options.addOption(Option("help", "h", "display help information on command line arguments")
		.required(false)
		.repeatable(false));

	options.addOption(Option("path", "p", "specify dump files base path").required(false).repeatable(false));
}

void DumpAnalyzer::handleOption(const string& name, const string& value)
{
	ServerApplication::handleOption(name, value);

	if (name == "help")
		_helpRequested = true;
	if (name == "path") {
		_basePath = value;
	}
}

void DumpAnalyzer::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.setHeader("A service automating analyze dump files.");
	helpFormatter.format(std::cout);
}

int DumpAnalyzer::main(const std::vector<std::string>& args)
{
	if (_helpRequested)
	{
		displayHelp();
	}
	else
	{
		AutoPtr<SimpleFileChannel> pChannel(new SimpleFileChannel);
		pChannel->setProperty("path", "analyzer.log");
		pChannel->setProperty("rotation", "2 K");
		Logger::root().setChannel(pChannel);

		this->setLogger(Logger::get("dump"));

		int tid = Poco::Thread::currentTid();
		string basePath = config().getString("basePath", "dumps");
		WindbgProcessor processor(basePath);

		this->logger().information(Poco::format("%s: Service will start in tid %d", Poco::DateTimeFormatter::format(Poco::LocalDateTime().timestamp(), "%Y-%m-%d %H:%M:%s"), tid));
		processor.Start();

		// wait for CTRL-C or kill
		waitForTerminationRequest();
		// Stop the HTTPServer
		processor.Stop();
	}
	return Application::EXIT_OK;
}
