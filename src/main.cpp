#include <crow.h>
#include <sqlite3.h>
#include "Account.hpp"
#include "db.hpp"

int main() {
    sqlite3* db;
    int rc = sqlite3_open("example.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    crow::SimpleApp app;
    std::unique_ptr<Account> accountPtr(new Account("John Doe", "johndoe@test.com", "password"));

    //prepare the database
    prepareDatabase(db);

    CROW_ROUTE(app, "/hello")
    ([]() {
        return "Hello, World!";
    });

    //health check
    CROW_ROUTE(app, "/health")
    ([]() {
        crow::json::wvalue x({{"status", "OK"}});
        return x;
    });

    //user POST method
    CROW_ROUTE(app, "/users")
    .methods("POST"_method)
    ([&db](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x) {
            return crow::response(400);
        }
        std::string email = x["email"].s();
        std::string password = x["password"].s();
        int age = x["age"].i();
        std::string sql = "INSERT INTO account (email, password, age) VALUES ('" + email + "', '" + password + "', " + std::to_string(age) + ")";
        int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
            return crow::response(500);
        }
        return crow::response(201);
    });

    //get all users
    CROW_ROUTE(app, "/users")
    ([&db](){
         //result in json format
        crow::json::wvalue x;

        const char* sql = "SELECT email, password, age FROM account";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
            x["error"] = std::string("Error preparing statement");
            return x;
        }
       
        int index = 0;
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            std::string email = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            std::string password = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            int age = sqlite3_column_int(stmt, 2);
            crow::json::wvalue user;
            user["email"] = email;
            user["password"] = password;
            user["age"] = age;
            x["users"][index++] = std::move(user);
        }
        sqlite3_finalize(stmt);
        return x;
    });

    //get a user by email
    CROW_ROUTE(app, "/users/<string>")
    ([&db](std::string email){
        std::string sql = "SELECT email, password, age FROM account WHERE email = '" + email + "'";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
            return std::string("Error preparing statement");
        }
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            std::string email = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            std::string password = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            int age = sqlite3_column_int(stmt, 2);
            sqlite3_finalize(stmt);
            return "Email: " + email + ", Password: " + password + ", Age: " + std::to_string(age);
        }
        sqlite3_finalize(stmt);
        return std::string("User not found");
    });

    //get user details
    CROW_ROUTE(app, "/class/user")
    ([&accountPtr](){
        crow::json::wvalue x;
        x["name"] = accountPtr->getName();
        x["email"] = accountPtr->getEmail();
        return x;
    });
    

    app.port(8080).multithreaded().run();

    sqlite3_close(db);
    
    return 0;
}
