#pragma once
#include <string>
#include "Poco\File.h"
#include "..\DumpLib\History.h"

class WindbgProcessor
{
public:
	WindbgProcessor(std::string basePath) :_basePath(basePath) {}
	~WindbgProcessor();

public:
	void Start();
	void Stop();

private:
	void RecurseDir(Poco::Path basePath);
	void ExecWindbg(Poco::Path dumpFile, Poco::Path base);
	void Log(std::string info);
	History GetResult(Poco::Path filePath);

	std::string _basePath;

};

