#include "Poco\Path.h"
#include "Poco\File.h"
#include <string>

#include "FileHelper.h"

using namespace std;
using namespace Poco;

string FileHelper::getExistsDir(string basePath, string subDir) {
	Path base;
	base = base.parse(basePath).makeAbsolute();

	File file(base);
	if (!file.exists()) {
		file.createDirectory();
	}

	if (subDir.length() > 0) {
		base.append(subDir);
		File subFile(base);
		if (!subFile.exists()) {
			subFile.createDirectory();
		}
	}
	return base.toString();
}
