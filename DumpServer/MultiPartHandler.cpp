#include "Poco\CountingStream.h"
#include "Poco\Path.h"
#include "Poco\FileStream.h"
#include "Poco\StreamCopier.h"
#include "Poco\NullStream.h"
#include "Poco\Net\PartHandler.h"
#include "Poco\Net\MessageHeader.h"
#include "Poco\UnicodeConverter.h"

#include "MultiPartHandler.h"

#include <string>
#include <iostream>

using namespace std;

using namespace Poco;
using namespace Poco::Net;
using Poco::UnicodeConverter;

void MultiPartHandler::handlePart(const MessageHeader& header, istream& stream)
{
	_isFile = true;
	_type = header.get("Content-Type", "(unspecified)");
	_fileName.clear();
	if (header.has("Content-Disposition"))
	{
		std::string disp;
		NameValueCollection params;
		MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
		_name = params.get("name", "(unnamed)");
		_fileName = params.get("filename", "(unnamed)");
		/*string umsg;
		UnicodeConverter::convert((const char*)_fileName, umsg);
		_fileName = umsg;*/
	}

	CountingInputStream istr(stream);
	if (_fileName.length() > 0) {
		Path filePath(_basePath);
		filePath = filePath.append(_fileName);
		filePath.setBaseName(_guid);
		FileStream ostr(filePath.toString());
		StreamCopier::copyStream(istr, ostr);
	}
	else {
		NullOutputStream ostr;
		StreamCopier::copyStream(istr, ostr);
	}
	_length = istr.chars();
}

int MultiPartHandler::length() const
{
	return _length;
}

const string& MultiPartHandler::name() const
{
	return _name;
}

const string& MultiPartHandler::fileName() const
{
	return _fileName;
}

const string& MultiPartHandler::contentType() const
{
	return _type;
}

const bool& MultiPartHandler::isFile() const
{
	return _isFile;
}