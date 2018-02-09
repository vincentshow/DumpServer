#pragma once
#include "Poco/SharedPtr.h"
#include "Poco/DateTime.h"
#include "Poco/Data/TypeHandler.h"
#include <string>

using std::string;
using std::size_t;
using Poco::DateTime;

struct History
{
	string   GUID;
	string   Product;
	string   Version;
	string   IP;
	DateTime Timestamp;
	string   DumpPath;
	string   FollowUp;
	string   SymbolName;
	string   StackText;
	DateTime LastUpdate;
};

namespace Poco {
	namespace Data {

		using namespace Poco::Data::Keywords;

		template <>
		class TypeHandler<History>
			/// Defining a specialization of TypeHandler for Person allows us
			/// to use the Person struct in use and into clauses.
		{
		public:
			static size_t size()
			{
				//must gurantee this number equals History property count
				return 10;
			}

			static void bind(size_t pos, const History& history, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
			{
				TypeHandler<string>::bind(pos++, history.GUID, pBinder, dir);
				TypeHandler<string>::bind(pos++, history.Product, pBinder, dir);
				TypeHandler<string>::bind(pos++, history.Version, pBinder, dir);
				TypeHandler<string>::bind(pos++, history.IP, pBinder, dir);
				TypeHandler<DateTime>::bind(pos++, history.Timestamp, pBinder, dir);
				TypeHandler<string>::bind(pos++, history.DumpPath, pBinder, dir);
				TypeHandler<string>::bind(pos++, history.FollowUp, pBinder, dir);
				TypeHandler<string>::bind(pos++, history.SymbolName, pBinder, dir);
				TypeHandler<string>::bind(pos++, history.StackText, pBinder, dir);
				TypeHandler<DateTime>::bind(pos++, history.LastUpdate, pBinder, dir);
			}

			static void extract(size_t pos, History& history, const History& deflt, AbstractExtractor::Ptr pExtr)
			{
				TypeHandler<string>::extract(pos++, history.GUID, deflt.GUID, pExtr);
				TypeHandler<string>::extract(pos++, history.Product, deflt.Product, pExtr);
				TypeHandler<string>::extract(pos++, history.Version, deflt.Version, pExtr);
				TypeHandler<string>::extract(pos++, history.IP, deflt.IP, pExtr);
				TypeHandler<DateTime>::extract(pos++, history.Timestamp, deflt.Timestamp, pExtr);
				TypeHandler<string>::extract(pos++, history.DumpPath, deflt.DumpPath, pExtr);
				TypeHandler<string>::extract(pos++, history.FollowUp, deflt.FollowUp, pExtr);
				TypeHandler<string>::extract(pos++, history.SymbolName, deflt.SymbolName, pExtr);
				TypeHandler<string>::extract(pos++, history.StackText, deflt.StackText, pExtr);
				TypeHandler<DateTime>::extract(pos++, history.LastUpdate, deflt.LastUpdate, pExtr);
			}

			static void prepare(size_t pos, const History& history, AbstractPreparator::Ptr pPrep)
			{
				TypeHandler<string>::prepare(pos++, history.GUID, pPrep);
				TypeHandler<string>::prepare(pos++, history.Product, pPrep);
				TypeHandler<string>::prepare(pos++, history.Version, pPrep);
				TypeHandler<string>::prepare(pos++, history.IP, pPrep);
				TypeHandler<DateTime>::prepare(pos++, history.Timestamp, pPrep);
				TypeHandler<string>::prepare(pos++, history.DumpPath, pPrep);
				TypeHandler<string>::prepare(pos++, history.FollowUp, pPrep);
				TypeHandler<string>::prepare(pos++, history.SymbolName, pPrep);
				TypeHandler<string>::prepare(pos++, history.StackText, pPrep);
				TypeHandler<DateTime>::prepare(pos++, history.LastUpdate, pPrep);
			}
		};
	}
} // namespace Poco::Data