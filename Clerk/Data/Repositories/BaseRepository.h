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

		protected:
			DataConnection& _connection;
		};
	}
}

