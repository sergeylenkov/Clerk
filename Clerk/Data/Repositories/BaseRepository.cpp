#include "BaseRepository.h"

using namespace Clerk::Data;

BaseRepository::BaseRepository(DataConnection& connection) : _connection(connection) {
	
}

sqlite3* BaseRepository::GetConnection() {
	return _connection.GetConnection();
}