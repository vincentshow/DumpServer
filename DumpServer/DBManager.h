#pragma once

#include "Poco\Data\Session.h"
#include "Poco\Data\SQLite\Notifier.h"
#include "Poco\Data\Statement.h"
#include <vector>

using Poco::Data::Session;
using Poco::Data::SQLite::Notifier;
using Poco::Data::Statement;

namespace Dump {
	namespace Model {
		using namespace Poco::Data::Keywords;

		class DBManager
			/// Handler for DB insert/update events.
		{
		public:
			~DBManager();
			static DBManager* Instance();

			template<typename T>
			std::size_t execute(const std::string& sql, T* data) {
				Poco::Data::Statement stmt(_session);
				if (data) {
					stmt << sql, use<T>(*data);
				}
				else {
					stmt << sql;
				}
				return stmt.execute();
			}

			template<typename T>
			std::vector<T> query(const std::string& sql) {
				std::vector<T> result;
				_session << sql, into(result), now;
				return result;
			}
		private:
			DBManager();
			static DBManager* _instance;

			Session                _session;
			Notifier               _notifier;

			void initDB();
			Notifier* notifier(const void* pSender);
			void notify(Poco::Int64 rowID);
			void onInsert(const void* pSender);
			void onUpdate(const void* pSender);
		};
	}
}