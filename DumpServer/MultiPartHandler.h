#pragma once
#include "Poco\Net\PartHandler.h"
#include <string>
#include <iostream>

using std::string;
using std::istream;
using Poco::Net::PartHandler;
using Poco::Net::MessageHeader;


class MultiPartHandler : public PartHandler
{
public:
	MultiPartHandler(string basePath, string guid) :_basePath(basePath), _guid(guid), _isFile(false), _length(0)
	{}
	void handlePart(const MessageHeader& header, istream& stream);
	int length() const;
	const string& name() const;
	const string& fileName() const;
	const string& contentType() const;
	const bool& isFile() const;

private:
	int _length;
	string _type;
	string _name;
	string _fileName;
	string _guid;
	string _basePath;
	bool _isFile;
};