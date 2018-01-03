#pragma once
#include "Poco\Net\HTTPRequestHandler.h"
#include <string>

using std::string;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

class FormRequestHandler : public HTTPRequestHandler
{
public:
	FormRequestHandler(string basePath);

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
private:
	string _basePath;
	string getStaticResult();
};