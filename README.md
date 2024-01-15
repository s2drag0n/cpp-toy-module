# C++组件集合

## 包含内容

- 线程池 [ThreadPool](./ThreadPool/)
- 内存池 [MemoryPool](./MemoryPool/)

## 线程池 ThreadPool

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

## 内存池 MemoryPool

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

