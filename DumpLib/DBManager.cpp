#include "stdafx.h"
#include "DBManager.h"
#include "Poco\Delegate.h"
#include "Poco\Data\RecordSet.h"
#include "Poco\Data\Session.h"
#include "Poco\Data\SQLite\Notifier.h"
#include "Poco\Data\SQLite\Connector.h"

using std::cout;
using std::endl;
using Poco::delegate;
using Poco::Data::Session;
using Poco::Data::SQLite::Notifier;
using Poco::Data::RecordSet;

using namespace Poco::Data::Keywords;

namespace Dump {
	namespace Model {

		DBManager* DBManager::_instance = nullptr;

		DBManager::DBManager() :
			_session("SQLite", "dump.db"),
			_notifier(_session)
			/// Constructor; opens/initializes the database and associates
			/// notification events with their respective handlers.
		{
			initDB();
			//_notifier.insert += delegate(this, &DBManager::onInsert);
			//_notifier.update += delegate(this, &DBManager::onUpdate);
		}

		DBManager::~DBManager()
			/// Destructor; unregisters the notification events.
		{
			//_notifier.insert -= delegate(this, &DBManager::onInsert);
			//_notifier.update -= delegate(this, &DBManager::onUpdate);

			//delete _instance;
			//_instance = NULL;
		}

		DBManager* DBManager::Instance() {
			if (NULL == _instance)
			{
				_instance = new DBManager();
			}
			return _instance;
		}

		void DBManager::initDB()
		{
			_session << "CREATE TABLE IF NOT EXISTS History (GUID VARCHAR(40) PRIMARY KEY, Product VARCHAR, Version VARCHAR, IP VARCHAR(30), Timestamp DATETIME, DumpPath VARCHAR(200), DumpResult Text, LastUpdate DATETIME)", now;
		}

		Notifier* DBManager::notifier(const void* pSender)
		{
			return reinterpret_cast<Notifier*>(const_cast<void*>(pSender));
		}

		void DBManager::notify(Poco::Int64 rowID)
			/// Executes the query and sends the data to the web client.
		{
			std::ostringstream os;
			Statement stmt = (_session << "SELECT * FROM History WHERE rowid = ?", use(rowID), now);
			os << RecordSet(stmt);
			cout << os.str() << endl;
		}

		void DBManager::onInsert(const void* pSender)
			/// Insert event handler; retrieves the data for the affected row
			/// and calls notify.
		{
			Notifier* pN = notifier(pSender);
			Poco::Int64 rowID = pN->getRow();
			std::cout << "Inserted row " << rowID << std::endl;
			notify(rowID);
		}

		void DBManager::onUpdate(const void* pSender)
			/// Update event handler; retrieves the data for the affected row
			/// and calls notify.
		{
			Notifier* pN = notifier(pSender);
			Poco::Int64 rowID = pN->getRow();
			std::cout << "Updated row " << rowID << std::endl;
			notify(rowID);
		}
	}
}