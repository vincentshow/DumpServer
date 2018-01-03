#pragma once
#include <string>

using std::string;

class FileHelper {
public:
	static string getExistsDir(string basePath, string subDir = "");
};