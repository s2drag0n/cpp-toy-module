#pragma once

#include </opt/homebrew/Cellar/mysql/8.2.0_1/include/mysql/mysql.h>
#include <chrono>

#define LOG(message)                                                           \
    std::cerr << __FILE__ << ": " << __LINE__ << ": " << __TIMESTAMP__ << ": " \
              << message << std::endl;

class SQLConnection {
  private:
    /* conntion instance of mysql */
    MYSQL *mysql;

    /* init the conntion */
    void init_connection(const char *host, unsigned port, const char *user,
                         const char *passwd, const char *db_name);

  public:
    SQLConnection(const char *host, const unsigned port, const char *user,
                  const char *passwd, const char *db_name);

    ~SQLConnection();

    /* reset the time_point when push */
    void reset_time_point();

    /* time_point when this conntion is free(pushed to the queue) */
    std::chrono::time_point<std::chrono::high_resolution_clock> start_point;

    /* execute a query sentence */
    MYSQL_RES *query(const char *query);
};
