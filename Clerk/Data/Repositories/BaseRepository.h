#pragma once

#include <memory>
#include <map>
#include <vector>
#include "../DataConnection.h"
#include "../Models/Model.h"

namespace Clerk {
	namespace Data {
		class BaseRepository
		{
		public:
			BaseRepository(DataConnection& connection);
			sqlite3* GetConnection();

		protected:
			DataConnection& _connection;
		};
	}
}

