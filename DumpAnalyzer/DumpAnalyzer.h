#pragma once
#include "Poco\Util\ServerApplication.h"
#include <vector>
#include <string>

using std::string;
using std::vector;
using Poco::Util::ServerApplication;
using Poco::Util::OptionSet;

class DumpAnalyzer : public ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	/// Start the HTTPFormServer executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
	///
	/// To use the sample configuration file (HTTPFormServer.properties),
	/// copy the file to the directory where the HTTPFormServer executable
	/// resides. If you start the debug version of the HTTPFormServer
	/// (HTTPFormServerd[.exe]), you must also create a copy of the configuration
	/// file named HTTPFormServerd.properties. In the configuration file, you
	/// can specify the port on which the server is listening (default
	/// 9980) and the format of the date/Form string sent back to the client.
	///
	/// To test the FormServer you can use any web browser (http://localhost:9980/).
{
public:
	DumpAnalyzer() : _helpRequested(false)
	{
	}

	~DumpAnalyzer()
	{
	}

protected:
	void initialize(Application& self);
	void uninitialize();
	void defineOptions(OptionSet& options);
	void handleOption(const string& name, const string& value);
	void displayHelp();
	int main(const vector<string>& args);

private:
	bool _helpRequested;
	std::string _basePath;
};
