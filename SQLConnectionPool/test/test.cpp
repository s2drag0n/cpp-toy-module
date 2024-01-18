#include "../SQLConnection.h"
#include "../SQLConnectionPool.h"
#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

int main() {

    /* 单线程 */
    /* 不使用连接池测试 */
    SQLConnectionPool::ConnectionConfig conn_config;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000; ++i) {

        SQLConnection sql_connection(conn_config.host, conn_config.port,
                                     conn_config.user, conn_config.passwd,
                                     conn_config.db_name);

        const char *sql = "select score from course where course_name = "
                          "\"Machine Learning\";";
        MYSQL_RES *res = sql_connection.query(sql);
        MYSQL_ROW row = mysql_fetch_row(res);
        unsigned long *lengths = mysql_fetch_lengths(res);
        std::string result_str(row[0], lengths[0]);
        assert(result_str == "92.30");
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    auto result = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    std::cout << "测试结果============================================"
              << std::endl;

    std::cout << "\t单线程, 连续查询1000次:" << std::endl;
    std::cout << "\t\t不使用连接池, 用时: " << result.count() << " ms"
              << std::endl;

    /* 使用连接池测试 */
    SQLConnectionPool *sql_connection_pool =
        SQLConnectionPool::get_connection_pool();

    start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000; ++i) {
        std::shared_ptr<SQLConnection> sql_connection_ptr =
            sql_connection_pool->get_connection();

        const char *sql = "select score from course where course_name = "
                          "\"Machine Learning\";";

        MYSQL_RES *res = sql_connection_ptr->query(sql);
        MYSQL_ROW row = mysql_fetch_row(res);
        unsigned long *lengths = mysql_fetch_lengths(res);
        std::string result_str(row[0], lengths[0]);
        assert(result_str == "92.30");
    }

    end_time = std::chrono::high_resolution_clock::now();

    auto result2 = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    std::cout << "\t\t使用连接池, 用时: " << result2.count() << " ms"
              << std::endl;

    std::cout << "\t使用连接池的效率是不使用连接池的 "
              << result.count() / result2.count() << " 倍" << std::endl
              << std::endl;

    /* 多线程 */
    /* 不使用连接池测试 */

    start_time = std::chrono::high_resolution_clock::now();
    std::thread t1([&conn_config]() {
        for (int i = 0; i < 250; ++i) {

            SQLConnection sql_connection(conn_config.host, conn_config.port,
                                         conn_config.user, conn_config.passwd,
                                         conn_config.db_name);

            const char *sql = "select score from course where course_name = "
                              "\"Machine Learning\";";
            MYSQL_RES *res = sql_connection.query(sql);
            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned long *lengths = mysql_fetch_lengths(res);
            std::string result_str(row[0], lengths[0]);
            assert(result_str == "92.30");
        }
    });
    std::thread t2([&conn_config]() {
        for (int i = 0; i < 250; ++i) {

            SQLConnection sql_connection(conn_config.host, conn_config.port,
                                         conn_config.user, conn_config.passwd,
                                         conn_config.db_name);

            const char *sql = "select score from course where course_name = "
                              "\"Machine Learning\";";
            MYSQL_RES *res = sql_connection.query(sql);
            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned long *lengths = mysql_fetch_lengths(res);
            std::string result_str(row[0], lengths[0]);
            assert(result_str == "92.30");
        }
    });
    std::thread t3([&conn_config]() {
        for (int i = 0; i < 250; ++i) {

            SQLConnection sql_connection(conn_config.host, conn_config.port,
                                         conn_config.user, conn_config.passwd,
                                         conn_config.db_name);

            const char *sql = "select score from course where course_name = "
                              "\"Machine Learning\";";
            MYSQL_RES *res = sql_connection.query(sql);
            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned long *lengths = mysql_fetch_lengths(res);
            std::string result_str(row[0], lengths[0]);
            assert(result_str == "92.30");
        }
    });
    std::thread t4([&conn_config]() {
        for (int i = 0; i < 250; ++i) {

            SQLConnection sql_connection(conn_config.host, conn_config.port,
                                         conn_config.user, conn_config.passwd,
                                         conn_config.db_name);

            const char *sql = "select score from course where course_name = "
                              "\"Machine Learning\";";
            MYSQL_RES *res = sql_connection.query(sql);
            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned long *lengths = mysql_fetch_lengths(res);
            std::string result_str(row[0], lengths[0]);
            assert(result_str == "92.30");
        }
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    end_time = std::chrono::high_resolution_clock::now();

    auto result3 = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    std::cout << "\t四个线程, 每个线程连续查询250次:" << std::endl;
    std::cout << "\t\t不使用连接池, 用时: " << result3.count() << " ms"
              << std::endl;

    /* 使用连接池测试 */
    SQLConnectionPool *sql_connection_pool1 =
        SQLConnectionPool::get_connection_pool();

    start_time = std::chrono::high_resolution_clock::now();

    std::thread tt1([&conn_config, sql_connection_pool1]() {
        for (int i = 0; i < 250; ++i) {
            std::shared_ptr<SQLConnection> sql_connection_ptr =
                sql_connection_pool1->get_connection();

            const char *sql = "select score from course where course_name = "
                              "\"Machine Learning\";";

            MYSQL_RES *res = sql_connection_ptr->query(sql);
            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned long *lengths = mysql_fetch_lengths(res);
            std::string result_str(row[0], lengths[0]);
            assert(result_str == "92.30");
        }
    });
    std::thread tt2([&conn_config, sql_connection_pool1]() {
        for (int i = 0; i < 250; ++i) {
            std::shared_ptr<SQLConnection> sql_connection_ptr =
                sql_connection_pool1->get_connection();

            const char *sql = "select score from course where course_name = "
                              "\"Machine Learning\";";

            MYSQL_RES *res = sql_connection_ptr->query(sql);
            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned long *lengths = mysql_fetch_lengths(res);
            std::string result_str(row[0], lengths[0]);
            assert(result_str == "92.30");
        }
    });
    std::thread tt3([&conn_config, sql_connection_pool1]() {
        for (int i = 0; i < 250; ++i) {
            std::shared_ptr<SQLConnection> sql_connection_ptr =
                sql_connection_pool1->get_connection();

            const char *sql = "select score from course where course_name = "
                              "\"Machine Learning\";";

            MYSQL_RES *res = sql_connection_ptr->query(sql);
            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned long *lengths = mysql_fetch_lengths(res);
            std::string result_str(row[0], lengths[0]);
            assert(result_str == "92.30");
        }
    });
    std::thread tt4([&conn_config, sql_connection_pool1]() {
        for (int i = 0; i < 250; ++i) {
            std::shared_ptr<SQLConnection> sql_connection_ptr =
                sql_connection_pool1->get_connection();

            const char *sql = "select score from course where course_name = "
                              "\"Machine Learning\";";

            MYSQL_RES *res = sql_connection_ptr->query(sql);
            MYSQL_ROW row = mysql_fetch_row(res);
            unsigned long *lengths = mysql_fetch_lengths(res);
            std::string result_str(row[0], lengths[0]);
            assert(result_str == "92.30");
        }
    });

    tt1.join();
    tt2.join();
    tt3.join();
    tt4.join();

    end_time = std::chrono::high_resolution_clock::now();

    auto result4 = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    std::cout << "\t\t使用连接池,  用时: " << result4.count() << " ms"
              << std::endl;

    std::cout << "\t使用连接池的效率是不使用连接池的 "
              << result3.count() / result4.count() << " 倍" << std::endl;

    std::cout << "===================================================="
              << std::endl;
    return 0;
}
