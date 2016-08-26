#ifndef Model_h
#define Model_h

#include <sqlite3.h>

class Model
{
public:
	static void SetConnection(sqlite3 *db);
	
	static sqlite3 *_db;

private:

};

#endif