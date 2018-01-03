#pragma once
#include "Poco\Net\HTTPRequestHandlerFactory.h"

#include "FormRequestHandler.h"

using std::string;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;


class FormRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	FormRequestHandlerFactory(string basePath) :_basePath(basePath)
	{
	}
	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);

private:
	string _basePath;
};