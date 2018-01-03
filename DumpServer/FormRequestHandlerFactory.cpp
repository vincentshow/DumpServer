#include "Poco\Net\HTTPRequestHandlerFactory.h"
#include "FormRequestHandler.h"
#include "FormRequestHandlerFactory.h"

using namespace Poco::Net;

HTTPRequestHandler* FormRequestHandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
	return new FormRequestHandler(_basePath);
}
