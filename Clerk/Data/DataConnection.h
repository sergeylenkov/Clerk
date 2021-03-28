#pragma once

#include <string>
#include <sqlite3.h>

namespace Clerk {
	namespace Data {
		class DataConnection {
		public:
			DataConnection(std::string&& path);
			~DataConnection();

			void Open();
			void Close();
			sqlite3* GetConnection();
			void CreateDatabase();

		private:
			std::string _path;
			sqlite3* _connection;
		};
	};
};