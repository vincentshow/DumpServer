#include "Poco\UUIDGenerator.h"
#include "Poco\Path.h"
#include "Poco\JSON\Object.h"
#include "Poco\Net\HTTPRequestHandler.h"
#include "Poco\Net\HTMLForm.h"
#include "Poco\Net\HTTPServerRequest.h"
#include "Poco\Net\HTTPServerResponse.h"
#include "Poco\Util\ServerApplication.h"
#include "FileHelper.h"
#include "FormRequestHandler.h"
#include "MultiPartHandler.h"
#include "..\DumpLib\DBManager.h"
#include "..\DumpLib\History.h"
#include <vector>
#include <iostream>
#include <fstream>

using std::string;
using std::vector;
using std::ofstream;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;
using Dump::Model::DBManager;

FormRequestHandler::FormRequestHandler(string basePath)
{
	_basePath = FileHelper::getExistsDir(basePath, DateTimeFormatter::format(LocalDateTime().timestamp(), "%Y%m%d"));
}

void FormRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	int tid = Poco::Thread::currentTid();
	string host = request.clientAddress().host().toString();

	Application& app = Application::instance();
	app.logger().information(Poco::format("%s: Request from %s in tid %d", Poco::DateTimeFormatter::format(Poco::LocalDateTime().timestamp(), "%Y-%m-%d %H:%M:%s"), host, tid));

	HTMLForm form;
	vector<string> mutipartContent;
	if (Poco::toLower(request.getMethod()) == "post") {
		string dumpPath = FileHelper::getExistsDir(_basePath, host);
		string guid = Poco::UUIDGenerator::defaultGenerator().create().toString();
		MultiPartHandler partHandler(dumpPath, guid);
		form.load(request, request.stream(), partHandler);
		if (partHandler.isFile()) {
			Poco::JSON::Object meta;
			meta.set("tid", tid);
			auto it = form.begin();
			for (; it != form.end(); ++it) {
				meta.set(it->first, it->second);
			}

			Poco::Path path(dumpPath, Poco::cat<string>(guid, ".json"));
			ofstream ostr(path.toString());
			meta.stringify(ostr);

			DateTime dt;
			dt.makeLocal(LocalDateTime().tzd());
			DBManager::Instance()->execute<History>("replace into history values(?, ?, ?, ?, ?, ?, ?, ?)", new History
			{
				guid,
				meta.has("product") ? meta.getValue<string>("product") : "",
				meta.has("version") ? meta.getValue<string>("version") : "",
				host,
				dt
			});

			mutipartContent.push_back(Poco::format("<h2>Upload</h2><p>%s", partHandler.name()));
			mutipartContent.push_back(Poco::format("<br>File Name: %s", partHandler.fileName()));
			mutipartContent.push_back(Poco::format("<br>Type: %s", partHandler.contentType()));
			mutipartContent.push_back(Poco::format("<br>Size:%d<br></p>", partHandler.length()));
		}
	}
	else {
		form.load(request);
	}

	response.setChunkedTransferEncoding(true);
	response.setContentType("text/html");

	std::ostream& ostr = response.send();

	ostr << getStaticResult();

	ostr << "<h2>Request</h2><p>\n";
	ostr << Poco::format(string("Method: %s <br>\n"), request.getMethod());
	ostr << Poco::format(string("URI: %s <br>\n"), request.getURI());
	auto it = request.begin();
	for (; it != request.end(); ++it)
	{
		ostr << it->first << ": " << it->second << "<br>\n";
	}
	ostr << "</p>";

	if (!form.empty())
	{
		ostr << "<h2>Form</h2><p>\n";
		it = form.begin();
		for (; it != form.end(); ++it)
		{
			ostr << it->first << ": " << it->second << "<br>\n";
		}
		ostr << "</p>";
	}

	if (!mutipartContent.empty())
	{
		ostr << Poco::cat(string(""), mutipartContent.begin(), mutipartContent.end());
	}
	ostr << "</body>\n";
}

string FormRequestHandler::getStaticResult() {
	return
		"<html>\n"
		"<head>\n"
		"<title>POCO Form Server Sample</title>\n"
		"</head>\n"
		"<body>\n"
		"<h1>POCO Form Server Sample</h1>\n"
		"<h2>GET Form</h2>\n"
		"<form method=\"GET\" action=\"/form\">\n"
		"<input type=\"text\" name=\"text\" size=\"31\">\n"
		"<input type=\"submit\" value=\"GET\">\n"
		"</form>\n"
		"<h2>POST Form</h2>\n"
		"<form method=\"POST\" action=\"/form\">\n"
		"<input type=\"text\" name=\"text\" size=\"31\">\n"
		"<input type=\"submit\" value=\"POST\">\n"
		"</form>\n"
		"<h2>File Upload</h2>\n"
		"<form method=\"POST\" action=\"/form\" enctype=\"multipart/form-data\">\n"
		"<input type=\"file\" name=\"file\" size=\"31\"> \n"
		"<input type=\"submit\" value=\"Upload\">\n"
		"</form>\n";
}



