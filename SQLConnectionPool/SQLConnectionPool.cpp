#include "SQLConnectionPool.h"
#include "SQLConnection.h"
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

SQLConnectionPool *SQLConnectionPool::m_instance = nullptr;

/* get the only object in the singleon pattern */
SQLConnectionPool *SQLConnectionPool::get_connection_pool() {
    if (m_instance != nullptr) {
        return m_instance;
    }
    m_instance = new SQLConnectionPool(5, 100, 50000, 3000);
    return m_instance;
}

/* get free connection from the free connection queue */
std::shared_ptr<SQLConnection> SQLConnectionPool::get_connection() {

    std::shared_ptr<SQLConnection> sql_connection = nullptr;

    {
        std::unique_lock<std::mutex> lock(m_mutex);

        while (m_connecion_queue.empty()) {
            // if no free connection in queue
            // notify produce thread it is time to produce
            produce_connection();

            if (cv.wait_for(lock, std::chrono::milliseconds(m_max_wait_time)) ==
                std::cv_status::timeout) {
                LOG("timeout");
                return sql_connection;
            }
        }

        // custom the destroy function to push it back after use
        sql_connection = std::shared_ptr<SQLConnection>(
            m_connecion_queue.front(), [&](SQLConnection *ptr) {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_connecion_queue.push(ptr);
                ++m_total_num_connection;
            });

        m_connecion_queue.pop();

        cv.notify_all();
    }

    return sql_connection;
}

SQLConnectionPool::SQLConnectionPool(int initial_num_connection,
                                     int max_num_connection, int max_free_time,
                                     int max_wait_time)
    : m_initial_num_connection(initial_num_connection),
      m_max_num_connection(max_num_connection), m_max_free_time(max_free_time),
      m_max_wait_time(max_wait_time), m_total_num_connection(0) {

    /* create m_initial_num_connection connection and push in queue, remeber
     * reset the time */
    for (int i = 0; i < initial_num_connection; ++i) {
        SQLConnection *sql_connection = new SQLConnection(
            m_conn_config.host, m_conn_config.port, m_conn_config.user,
            m_conn_config.passwd, m_conn_config.db_name);
        sql_connection->reset_time_point();
        m_connecion_queue.push(sql_connection);
        m_total_num_connection++;
    }

    // /* thread produce_connection */
    // std::thread produce_thread(&SQLConnectionPool::produce_connection);
    // produce_thread.detach();

    /* thread control free connection */
    std::thread time_thread([&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(m_mutex);

            cv.wait(lock, [this]() {
                return this->m_connecion_queue.size() >
                       this->m_initial_num_connection;
            });

            SQLConnection *sql_connection = m_connecion_queue.front();
            auto time_interval =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() -
                    sql_connection->start_point);

            if (time_interval >= std::chrono::milliseconds(m_max_free_time)) {
                // timeout connetion
                m_connecion_queue.pop();
                delete sql_connection;
            }
        }
    });
    time_thread.detach();
}

SQLConnectionPool::~SQLConnectionPool() {}

/* produce connetion */
void SQLConnectionPool::produce_connection() {

    std::unique_lock<std::mutex> lock(m_mutex);
    // if not reach the max_num_connection && no free connection to use
    // produce one
    while (m_connecion_queue.size() < m_max_num_connection &&
           m_total_num_connection == 0) {
        cv.wait(lock);
    }

    SQLConnection *sql_connection = new SQLConnection(
        m_conn_config.host, m_conn_config.port, m_conn_config.user,
        m_conn_config.passwd, m_conn_config.db_name);
    sql_connection->reset_time_point();
    m_connecion_queue.push(sql_connection);
    m_total_num_connection++;

    cv.notify_all();
}
