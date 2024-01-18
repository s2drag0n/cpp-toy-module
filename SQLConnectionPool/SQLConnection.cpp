#include "SQLConnection.h"
#include <chrono>
#include <cstdlib>
#include <iostream>

SQLConnection::SQLConnection(const char *host, unsigned port, const char *user,
                             const char *passwd, const char *db_name)
    : mysql(nullptr) {
    init_connection(host, port, user, passwd, db_name);
}

SQLConnection::~SQLConnection() { mysql_close(mysql); }

/* init the conntion */
void SQLConnection::init_connection(const char *host, const unsigned port,
                                    const char *user, const char *passwd,
                                    const char *db_name) {

    mysql = mysql_init(nullptr);
    if (mysql == nullptr) {
        LOG("mysql initial error");
        exit(1);
    }

    if (mysql_real_connect(mysql, host, user, passwd, db_name, port, nullptr,
                           0) == nullptr) {
        LOG("mysql connection error");
        exit(1);
    }
}

/* reset the time_point when push */
void SQLConnection::reset_time_point() {
    start_point = std::chrono::high_resolution_clock::now();
}

/* execute a query sentence */
MYSQL_RES *SQLConnection::query(const char *query) {
    if (mysql_query(mysql, query) != 0) {
        LOG("mysql query error");
        return nullptr;
    }

    return mysql_store_result(mysql);
}
