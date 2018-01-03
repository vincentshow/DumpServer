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
	string   Product;
	string   Version;
	string   IP;
	string   GUID;
	DateTime Timestamp;
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
				return 5;
			}

			static void bind(size_t pos, const History& history, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
			{
				TypeHandler<string>::bind(pos++, history.Product, pBinder, dir);
				TypeHandler<string>::bind(pos++, history.Version, pBinder, dir);
				TypeHandler<string>::bind(pos++, history.IP, pBinder, dir);
				TypeHandler<string>::bind(pos++, history.GUID, pBinder, dir);
				TypeHandler<DateTime>::bind(pos++, history.Timestamp, pBinder, dir);
			}

			static void extract(size_t pos, History& history, const History& deflt, AbstractExtractor::Ptr pExtr)
			{
				TypeHandler<string>::extract(pos++, history.Product, deflt.Product, pExtr);
				TypeHandler<string>::extract(pos++, history.Version, deflt.Version, pExtr);
				TypeHandler<string>::extract(pos++, history.IP, deflt.IP, pExtr);
				TypeHandler<string>::extract(pos++, history.GUID, deflt.GUID, pExtr);
				TypeHandler<DateTime>::extract(pos++, history.Timestamp, deflt.Timestamp, pExtr);
			}

			static void prepare(size_t pos, const History& history, AbstractPreparator::Ptr pPrep)
			{
				TypeHandler<string>::prepare(pos++, history.Product, pPrep);
				TypeHandler<string>::prepare(pos++, history.Version, pPrep);
				TypeHandler<string>::prepare(pos++, history.IP, pPrep);
				TypeHandler<string>::prepare(pos++, history.GUID, pPrep);
				TypeHandler<DateTime>::prepare(pos++, history.Timestamp, pPrep);
			}
		};
	}
} // namespace Poco::Data