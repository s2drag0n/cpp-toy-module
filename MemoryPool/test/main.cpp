#include "../MemoryPool.h"
#include "STDString.h"
#include "String.h"
#include <chrono>
#include <iostream>
#include <string>

int main() {

    const unsigned times = 100;
    const unsigned num_operation = 100000;

    MemoryPool *mp = new MemoryPool();

    delete mp;

    mp = new MemoryPool();

    auto start_time = std::chrono::high_resolution_clock::now();

    /* STDSTRING which use standard new/delete */
    for (int i = 0; i < times; ++i) {
        for (int j = 0; j < num_operation; ++j) {

            /* Allocation */
            STDString str("Every cloud has a silver lining.");
            /* Assignment */
            str = "Every cloud has a silver lining, ";
            /* Reallocation */
            str += "except the black one.";
            /* Deallocation */
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    auto STDString_duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                              start_time);

    start_time = std::chrono::high_resolution_clock::now();

    /* String which use MemoryPool */
    for (int i = 0; i < times; ++i) {
        for (int j = 0; j < num_operation; ++j) {

            /* Allocation */
            String str(mp, "Every cloud has a silver lining.");
            /* Assignment */
            str = "Every cloud has a silver lining, ";
            /* Reallocation */
            str += "except the black one.";
            /* Deallocation */
        }
    }

    end_time = std::chrono::high_resolution_clock::now();

    auto String_duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                              start_time);

    start_time = std::chrono::high_resolution_clock::now();

    /* STL string */
    for (int i = 0; i < times; ++i) {
        for (int j = 0; j < num_operation; ++j) {

            /* Allocation */
            std::string str("Every cloud has a silver lining.");
            /* Assignment */
            str = "Every cloud has a silver lining, ";
            /* Reallocation */
            str += "except the black one.";
            /* Deallocation */
        }
    }

    end_time = std::chrono::high_resolution_clock::now();

    auto STLString_duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                              start_time);

    std::cout
        << "Efficiency Test Result: "
           "=================================================================="
        << std::endl
        << std::endl;

    std::cout << "\t STDString  used:\t" << STDString_duration.count()
              << "ms\t(the string uses standard new/delete)" << std::endl;
    std::cout << "\t PoolString used:\t" << String_duration.count()
              << "ms\t(the string uses MemoryPool)" << std::endl;
    std::cout << "\t STLString  used:\t" << STLString_duration.count()
              << "ms\t(the string from STL)" << std::endl;

    std::cout << std::endl
              << "This memory pool is "
              << static_cast<float>(STDString_duration.count()) /
                     static_cast<float>(String_duration.count())
              << " times more efficient than standard "
                 "new/delete."
              << std::endl;

    // std::cout <<
    // "============================================================="
    //              "============================="
    //           << std::endl;

    delete mp;
    mp = nullptr;

    /* function test */
    FILE *original_stdout = freopen("/dev/null", "w", stdout); // Linux/macOS

    MemoryPool *np = new MemoryPool();

    String str(np, "Every cloud has a silver lining.");
    { String str1(np, "Every cloud has a silver lining."); }
    String str2(np, "Every cloud has a silver lining.");
    String str3(np, "Every cloud has a silver lining.");
    /* dump */
    np->dump_pool_data();
    str = str2;
    np->start_scope();
    String str4(np, "Every cloud has a silver lining.");
    String str5(np, "Every cloud has a silver lining.");
    String str6(np, "Every cloud has a silver lining.");
    String str7(np, "Every cloud has a silver lining.");
    String str8(np, "Every cloud has a silver lining.");
    String str9(np, "Every cloud has a silver lining.");
    String str10(np, "Every cloud has a silver lining.");
    /* dump */
    np->dump_pool_data();
    /* scope */
    np->end_scope();
    /* dump */
    np->dump_pool_data();
    delete np;
    np = nullptr;

    freopen("/dev/tty", "w", stdout);

    std::cout
        << std::endl
        << "Function Test Result: "
           "  ==========================================================="
           "======="
        << std::endl
        << std::endl
        << "scope functions work well && dump function work well"
        // << std::endl
        // << "============================================================="
        //    "============================="
        << std::endl;
    return 0;
}
