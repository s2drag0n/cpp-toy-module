#pragma once

#include "SQLConnection.h"
#include <condition_variable>
#include <memory>
#include <mutex>

#include <queue>

class SQLConnectionPool {
  public:
    /* get the only object in the singleon pattern */
    static SQLConnectionPool *get_connection_pool();

    /* get free connection from the free connection queue */
    std::shared_ptr<SQLConnection> get_connection();

    struct ConnectionConfig {
        /* 读取配置文件并初始化连接信息 */
        const char *host = "127.0.0.1";
        const char *user = "root";
        const char *passwd = "001023";
        const char *db_name = "szl_postgradual_courses";
        const unsigned port = 3306;
    };

  private:
    SQLConnectionPool(int initial_num_connection, int max_num_connection,
                      int max_free_time, int max_wait_time);

    ~SQLConnectionPool();

    static SQLConnectionPool *m_instance;

    /* initial num of connection */
    int m_initial_num_connection;

    /* max mun of connection */
    int m_max_num_connection;

    /* max time each connction can be stay in queue */
    int m_max_free_time;

    /* max time each every query last */
    int m_max_wait_time;

    /* total num of connection */
    std::atomic_int m_total_num_connection;

    /* connection queue */
    std::queue<SQLConnection *> m_connecion_queue;

    /* config of conn */
    struct ConnectionConfig m_conn_config;

    /* mutex */
    std::mutex m_mutex;

    /* conditional varirable */
    std::condition_variable cv;

    /* produce connetion */
    void produce_connection();
};
