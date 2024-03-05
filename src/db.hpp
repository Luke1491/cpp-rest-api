#ifndef DB_HPP
#define DB_HPP

#include <sqlite3.h> 

void createTable(sqlite3* db);
void prepareDatabase(sqlite3* db);

#endif // DB_HPP