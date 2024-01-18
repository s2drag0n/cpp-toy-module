# C++组件集合

```cpp
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                              8            181             61            680
C/C++ Header                     6             95             39            222
Markdown                         1             37              0             88
CMake                            3             10              0             19
SQL                              1              3              4             14
-------------------------------------------------------------------------------
SUM:                            19            326            104           1023
-------------------------------------------------------------------------------
```

## 包含内容

- 线程池 [ThreadPool](./ThreadPool/)
- 内存池 [MemoryPool](./MemoryPool/)
- SQL数据库连接池[SQLConnectionPool](./SQLConnectionPool/)

# 线程池

FROM [Here](https://github.com/progschj/ThreadPool)

使用C++11新特性实现的跨平台线程池。

### 基本用法：

将ThreadPool.h复制到项目中。

```c++
// 建立一个有4个工作线程的线程池
ThreadPool pool(4);

// 插入任务
auto result = pool.enqueue([](int answer) { return answer; }, 42);

// 获取返回结果
std::cout << result.get() << std::endl;
```

# 内存池

FROM [Here](https://github.com/DevShiftTeam/AppShift-MemoryPool/tree/main)

### 基本用法

 将MemoryPool.cpp 和 MemoryPool.h 文件复制到项目中。

```cpp
// 建立内存池
MemoryPool* mp = new MemorePool();

// 分配内存
youClass* obj = new (mp) youClass();
youClass* obj = new (mp) youClass()[num];

// 释放内存
mp->free(obj);

// 打印内存池使用情况
mp->dump_pool_data();

// 使用scope功能
mp->start_scope()
/* your code */
mp->end_scope();

// 删除内存池
delete mp;
```

### 压力测试

```
Efficiency Test Result: ==================================================================

	 STDString  used:	844ms	(the string uses standard new/delete)
	 PoolString used:	364ms	(the string uses MemoryPool)
	 STLString  used:	708ms	(the string from STL)

This memory pool is 2.31868 times more efficient than standard new/delete.

Function Test Result:   ==================================================================

scope functions work well && dump function work well
```

# 数据库连接池

服务器在向数据库发起每一次请求都要新建一次连接，这个过程非常缓慢（可以从后面的压力测试中看出来）。所以使用数据库连接池维护一些长期保持的连接，供给服务器的请求使用。每次使用时只需要从池子中取出一个连接，然后完成相应的增删改查任务即可，相应的连接将自动被放回连接池中。

### 基本用法

```cpp
// 新建连接池
SQLConnectionPool *sql_connection_pool1 =
        SQLConnectionPool::get_connection_pool();

// 从连接池中取出连接
std::shared_ptr<SQLConnection> sql_connection_ptr =
                sql_connection_pool1->get_connection();

// 执行各种sql语句
const char *sql = "select score from course where course_name = "
                              "\"Machine Learning\";";

// 获得结果
MYSQL_RES *res = sql_connection_ptr->query(sql);
```

### 可调整参数

- 连接池中**初始连接数量**
- 连接池可维护的**最大连接数量**
- **空闲连接最长存活时间**（需要保证空闲连接数不少于初始连接数量）
- **请求超时时间**（在非常高的并发情况下，连接池已经到达了最大连接数量，但是还是有新的获取连接请求到来，请求最多等待的时长，超时返回空指针）

### 压力测试

```cpp
测试结果============================================
	 单线程, 连续查询1000次:
		  不使用连接池, 用时: 11811 ms
		  使用连接池, 一千次连续查询用时: 56 ms
	 使用连接池的效率是不使用连接池的 210 倍

	 四个线程, 每个线程连续查询250次:
		  不使用连接池, 用时: 2058 ms
		  使用连接池, 每个线程连续查询250次: 24 ms
	 使用连接池的效率是不使用连接池的 85 倍
====================================================
```

