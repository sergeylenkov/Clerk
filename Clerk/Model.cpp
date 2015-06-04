#include "Model.h"

sqlite3 *Model::_db;

void Model::SetConnection(sqlite3 *db) {
	_db = db;
}