#pragma once
#include <string>
#include "Poco\File.h"
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
	std::string GetResult(std::string filePath);
	void Log(std::string info);

	std::string _basePath;

};

