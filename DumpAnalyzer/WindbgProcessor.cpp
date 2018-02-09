#include "WindbgProcessor.h"
#include "Poco\File.h"
#include "Poco\Process.h"
#include "Poco\DirectoryIterator.h"
#include "Poco\String.h"
#include "Poco\LocalDateTime.h"
#include "Poco\Util\Application.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include <fstream>
#include "..\DumpLib\DBManager.h"
#include "..\DumpLib\History.h"
#include "..\DumpServer\FileHelper.h"

using Poco::Process;
using Poco::ProcessHandle;
using Poco::DirectoryIterator;

WindbgProcessor::~WindbgProcessor()
{
}

void WindbgProcessor::Start() {
	Poco::Path base;
	base = base.parse(this->_basePath).makeAbsolute();

	this->_basePath = base.toString();

	Poco::File dir(this->_basePath);
	if (!dir.exists() || !dir.isDirectory()) {
		Log(Poco::format("no dir found in %s", this->_basePath));
		return;
	}
	this->RecurseDir(dir.path());
}

void WindbgProcessor::Stop() {
	std::cout << "stopped" << std::endl;
}

void WindbgProcessor::RecurseDir(Poco::Path base) {

	DirectoryIterator it(base);
	DirectoryIterator end;
	std::string relatedPath;
	History history;
	std::string logPath;

	while (it != end)
	{
		try
		{
			if (it->isDirectory()) {
				this->RecurseDir(it.path());
			}
			else if (it->isFile() && it.path().getExtension() == "dmp")
			{
				logPath = Poco::cat(it.path().toString(), std::string(".log"));
				if (!Poco::File(logPath).exists()) {
					//analyze dump and generate log file
					this->ExecWindbg(it.path(), base);
				}

				//get analyze result
				History history = WindbgProcessor::GetResult(logPath);

				//get related path
				relatedPath = Poco::replace(it.path().toString(), this->_basePath, std::string(""));

				//update db record
				std::string s = Poco::format("update history set dumpPath='%s', followUp='%s', symbolName='%s', stackText='%s', lastUpdate='%s' where guid='%s'",
					relatedPath,
					Poco::replace(history.FollowUp, "'", "''"),
					Poco::replace(history.SymbolName, "'", "''"),
					Poco::replace(history.StackText, "'", "''"),
					Poco::DateTimeFormatter::format(Poco::LocalDateTime().timestamp(), "%Y-%m-%d %H:%M:%s"),
					it.path().getBaseName());

				Log(Poco::format("followup of %s is: %s", relatedPath, history.FollowUp));

				Dump::Model::DBManager::Instance()->execute<History>(s, nullptr);
			}
		}
		catch (const std::exception& ex)
		{
			Log(ex.what());
		}
		++it;
	}
	Log(Poco::format("finished analyze dir %s", base.toString()));
}

void WindbgProcessor::ExecWindbg(Poco::Path dumpFile, Poco::Path base) {

	std::vector<std::string> args;
	args.push_back(dumpFile.getFileName());

	ProcessHandle ph = Process::launch(Poco::cat(base.current(), std::string("\\analyze.bat")), args, base.toString());
	ph.wait();
}

History WindbgProcessor::GetResult(Poco::Path filePath) {
	History history;

	std::vector<std::string> result;
	Poco::Path log(filePath);

	std::string lineContent;
	std::ifstream logStream(filePath.toString());
	bool canWrite(false);
	bool isFollowUp(false);
	bool isSymbol(false);
	bool isStack(false);

	try
	{
		while (std::getline(logStream, lineContent))
		{
			if (canWrite) {
				canWrite = lineContent.length() > 0;
			}
			else
			{
				isFollowUp = lineContent.find(std::string("FOLLOWUP_IP")) != std::string::npos;
				isSymbol = lineContent.find(std::string("SYMBOL_NAME")) != std::string::npos;
				isStack = lineContent.find(std::string("STACK_TEXT")) != std::string::npos;
				canWrite = isFollowUp || isSymbol || isStack;
			}

			if (canWrite) {
				lineContent += "\r\n";
				if (isFollowUp) {
					history.FollowUp += lineContent;
				}

				if (isSymbol) {
					history.SymbolName += lineContent;
				}

				if (isStack) {
					history.StackText += lineContent;
				}
			}
		}
		logStream.close();
	}
	catch (const std::exception& ex)
	{
		Log(ex.what());

		if (logStream.is_open()) {
			logStream.close();
		}
	}

	return history;
}

void WindbgProcessor::Log(std::string info) {
	std::string timestamp = Poco::DateTimeFormatter::format(Poco::LocalDateTime().timestamp(), "%Y-%m-%d %H:%M:%s");
	std::cout << timestamp << ":" << info << std::endl;
	Poco::Util::Application::instance().logger().log(Poco::cat(timestamp, std::string(":"), info));
}