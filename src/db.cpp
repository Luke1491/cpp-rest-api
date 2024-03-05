#include "db.hpp"
#include <iostream>
#include <sqlite3.h>
#include <string>

void mockUsers(sqlite3* db);

//function to create new table if no exists
void createTable(sqlite3* db) {
    const char* sql = "CREATE TABLE IF NOT EXISTS account (id INTEGER PRIMARY KEY, email TEXT, password TEXT, age INTEGER)";
    int rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(1);
    }

}

 void prepareDatabase(sqlite3* db) {
    int rc = sqlite3_open("example.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }
    createTable(db);
    mockUsers(db);
 }

 //put 1000 example users if account table is empty
    void mockUsers(sqlite3* db) {
        const char* sql = "SELECT COUNT(*) FROM account";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            std::cerr << "Error getting count: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            exit(1);
        }
        int count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
        if (count == 0) {
            for (int i = 0; i < 1000; i++) {
                std::string email = "user" + std::to_string(i) + "@example.com";
                std::string password = "password" + std::to_string(i);
                std::string sql = "INSERT INTO account (email, password, age) VALUES ('" + email + "', '" + password + "', 30)";
                rc = sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
                if (rc != SQLITE_OK) {
                    std::cerr << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
                    sqlite3_close(db);
                    exit(1);
                }
            }
        }
 }
